
**********/


#include "event_timer.hpp"

#include <iostream>
#include <memory>
#include <string>

// Xilinx OpenCL and XRT includes
#include "xilinx_ocl.hpp"

#define lm 8
#define ln 8
#define lp 8

#define m (1<<lm)
#define n (1<<ln)
#define p (1<<lp)

#ifdef HW_EMU
#define BUFSIZE m*n // = 256*256 elements for array
#else
#define BUFSIZE (1024 * 1024 * 32)
#endif

void vadd_sw(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t size)
{
    for(int i = 0; i < n; i++){

    for(int j = 0; j < p; j++){

        int result = 0;
        for(int k = 0; k < m; k++){
            result += a[i*m+k]*b[k*p+j];

        }
        c[i*p+j] = result;
        
    }
}
}

int main(int argc, char *argv[])
{
    // Initialize an event timer we'll use for monitoring the application
    EventTimer et;

    std::cout << "-- Parallelizing the Data Path --" << std::endl
              << std::endl;

    // Initialize the runtime (including a command queue) and load the
    // FPGA image
    std::cout << "Loading wide_vadd_example.xclbin to program the board" << std::endl
              << std::endl;
    et.add("OpenCL Initialization");

    // This application will use the first Xilinx device found in the system
    swm::XilinxOcl xocl;
    xocl.initialize("wide_vadd_example.xclbin");

    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl    = xocl.get_kernel("wide_vadd");
    et.finish();

    /// New code for example 01
    std::cout << "Running kernel test XRT-allocated buffers and wide data path:" << std::endl
              << std::endl;

    // Map our user-allocated buffers as OpenCL buffers using a shared
    // host pointer
    et.add("Allocate contiguous OpenCL buffers");
    cl_mem_ext_ptr_t bank_ext;
    bank_ext.flags = 2 | XCL_MEM_TOPOLOGY;
    bank_ext.obj   = NULL;
    bank_ext.param = 0;
    cl::Buffer a_buf(xocl.get_context(),
                     static_cast<cl_mem_flags>(CL_MEM_READ_ONLY),
                     BUFSIZE * sizeof(uint32_t), //BUFSIZE=1024*32
                     NULL,
                     NULL);
    cl::Buffer b_buf(xocl.get_context(),
                     static_cast<cl_mem_flags>(CL_MEM_READ_ONLY),
                     BUFSIZE * sizeof(uint32_t),
                     NULL,
                     NULL);
    cl::Buffer c_buf(xocl.get_context(),
                     static_cast<cl_mem_flags>(CL_MEM_READ_WRITE),
                     BUFSIZE * sizeof(uint32_t),
                     NULL,
                     NULL);
    cl::Buffer d_buf(xocl.get_context(),
                     static_cast<cl_mem_flags>(CL_MEM_READ_WRITE |
                                               CL_MEM_ALLOC_HOST_PTR |
                                               CL_MEM_EXT_PTR_XILINX),
                     BUFSIZE * sizeof(uint32_t),
                     &bank_ext,
                     NULL);
    et.finish();

    // Set vadd kernel arguments. We do this before mapping the buffers to allow XRT
    // to allocate the buffers in the appropriate memory banks for the selected
    // kernels. For buffer 'd' we explicitly set a bank above, but this buffer is
    // never migrated to the Alveo card so this mapping is theoretical.
    et.add("Set kernel arguments");
    krnl.setArg(0, a_buf);
    krnl.setArg(1, b_buf);
    krnl.setArg(2, c_buf);
    krnl.setArg(3, BUFSIZE);

    et.add("Map buffers to userspace pointers");
    uint32_t *a = (uint32_t *)q.enqueueMapBuffer(a_buf,
                                                 CL_TRUE,
                                                 CL_MAP_WRITE,
                                                 0,
                                                 BUFSIZE * sizeof(uint32_t));
    uint32_t *b = (uint32_t *)q.enqueueMapBuffer(b_buf,
                                                 CL_TRUE,
                                                 CL_MAP_WRITE,
                                                 0,
                                                 BUFSIZE * sizeof(uint32_t));
    uint32_t *d = (uint32_t *)q.enqueueMapBuffer(d_buf,
                                                 CL_TRUE,
                                                 CL_MAP_WRITE | CL_MAP_READ,
                                                 0,
                                                 BUFSIZE * sizeof(uint32_t));
    et.finish();

    et.add("Populating buffer inputs");
    for (int i = 0; i < BUFSIZE; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }
    et.finish();

    // For comparison, let's have the CPU calculate the result
    et.add("Software VADD run");
    vadd_sw(a, b, d, BUFSIZE);
    et.finish();

    // Send the buffers down to the Alveo card
    et.add("Memory object migration enqueue");
    cl::Event event_sp;
    q.enqueueMigrateMemObjects({a_buf, b_buf}, 0, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    et.add("OCL Enqueue task");

    q.enqueueTask(krnl, NULL, &event_sp);
    et.add("Wait for kernel to complete");
    clWaitForEvents(1, (const cl_event *)&event_sp);

    // Migrate memory back from device
    et.add("Read back computation results");
    uint32_t *c = (uint32_t *)q.enqueueMapBuffer(c_buf,
                                                 CL_TRUE,
                                                 CL_MAP_READ,
                                                 0,
                                                 BUFSIZE * sizeof(uint32_t));
    et.finish();


    // Verify the results
    bool verified = true;
    for (int i = 0; i < BUFSIZE; i++) {
        if (c[i] != d[i]) {
            verified = false;
            std::cout << "ERROR: software and hardware vadd do not match: "
                      << c[i] << "!=" << d[i] << " at position " << i << std::endl;
            break;
        }
    }

    if (verified) {
        std::cout
            << std::endl
            << "OCL-mapped contiguous buffer example complete!"
            << std::endl
            << std::endl;
    }
    else {
        std::cout
            << std::endl
            << "OCL-mapped contiguous buffer example complete! (with errors)"
            << std::endl
            << std::endl;
    }

    std::cout << "--------------- Key execution times ---------------" << std::endl;


    q.enqueueUnmapMemObject(a_buf, a);
    q.enqueueUnmapMemObject(b_buf, b);
    q.enqueueUnmapMemObject(c_buf, c);
    q.enqueueUnmapMemObject(d_buf, d);
    q.finish();


    et.print();
}
