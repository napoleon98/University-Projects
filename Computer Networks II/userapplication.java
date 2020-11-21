import java.net.*;
import java.util.ArrayList;
import java.io.*;
import javax.sound.sampled.*;

public class userapplication {

	static double packetCounter = 0;
	
	static long sumResponseTimes;
	
	static ArrayList<Integer> responseTimes = new ArrayList<Integer>();

	/**
	 * @param args
	 * @throws IOException
	 * @throws LineUnavailableException 
	 */
	public static void main(String[] args) throws IOException, LineUnavailableException {

		userapplication echopackets = new userapplication();
															// echo

		// txts
		PrintWriter responseTimesEcho = new PrintWriter("responseTimesEcho.txt", "UTF-8");
		PrintWriter responseTimesEchoWithoutDelay = new PrintWriter("responseTimesEchoWithoutDelay.txt", "UTF-8");
		PrintWriter temper = new PrintWriter("temper.txt", "UTF-8");
		PrintWriter ithCoptLmotor = new PrintWriter("ithCoptLmotor.txt", "UTF-8");
		PrintWriter ithCoptRmotor = new PrintWriter("ithCoptRmotor.txt", "UTF-8");
		PrintWriter ithCoptAlt = new PrintWriter("ithCoptAlt.txt", "UTF-8");
		PrintWriter ithCoptTemp = new PrintWriter("ithCoptTemp.txt", "UTF-8");
		PrintWriter ithCoptPress = new PrintWriter("ithCoptPress.txt", "UTF-8");
		PrintWriter throughPut = new PrintWriter("throughPut.txt", "UTF-8"); 
		PrintWriter throughPutWithoutDelay = new PrintWriter("throughPutWithoutDelay.txt", "UTF-8"); 
		
		PrintWriter DPCMsamplesFreq = new PrintWriter("DPCMsamplesFreq.txt", "UTF-8"); 
		PrintWriter DPCMsamplesClip = new PrintWriter("DPCMsamplesClip.txt", "UTF-8"); 
		PrintWriter DPCMDiffFs = new PrintWriter("DPCMDiffFs.txt", "UTF-8");
		PrintWriter DPCMDiffFs2 = new PrintWriter("DPCMDiffFs2.txt", "UTF-8");
		
	
		PrintWriter AQDPCMsamples1 = new PrintWriter("AQDPCMsamples1.txt", "UTF-8"); 
		
		
		PrintWriter AQDPCMDiffs1 = new PrintWriter("AQDPCMDiffs1.txt", "UTF-8");
		
		PrintWriter AQDPCMDiffs2 = new PrintWriter("AQDPCMDiffs2.txt", "UTF-8");
		PrintWriter AQDPCMsamples2 = new PrintWriter("AQDPCMsamples2.txt", "UTF-8"); 
		
		PrintWriter AQDPCMstepClip1 = new PrintWriter("AQDPCMstepClip1.txt", "UTF-8"); 
		PrintWriter AQDPCMstepClip2 = new PrintWriter("AQDPCMstepClip2.txt", "UTF-8"); 
		
		PrintWriter AQDPCMmeanClip1 = new PrintWriter("AQDPCMmeanClip1.txt", "UTF-8"); 
		PrintWriter AQDPCMmeanClip2 = new PrintWriter("AQDPCMmeanClip2.txt", "UTF-8"); 
		
		
		PrintWriter OBDEngRunTime = new PrintWriter("OBDEngRunTime.txt", "UTF-8");
		PrintWriter OBDAirTemp = new PrintWriter("OBDAirTemp.txt", "UTF-8");
		PrintWriter OBDThrPos = new PrintWriter("OBDThrPos.txt", "UTF-8");
		PrintWriter OBDEngRPM = new PrintWriter("OBDEngRPM.txt", "UTF-8");
		PrintWriter OBDVehSpeed = new PrintWriter("OBDVehSpeed.txt", "UTF-8");
		PrintWriter OBDCoolTemp = new PrintWriter("OBDCoolTemp.txt", "UTF-8");

		
		FileOutputStream im = new FileOutputStream("E1.jpg");
		FileOutputStream im2 = new FileOutputStream("E2.jpg");
		
		
		
		
		
		String EchoCode = "E0815";
		String EchoCodeWithoutDelay = "E0000";
		String imageCode = "M1415FLOW=ON";
		String imageCode2 = "M1415CAM=PTZFLOW=ON";
		String EchoCode2 = EchoCode + "T00";
		String audioCode = "A0998" + "L10F999"; 
		String audioCodeFreq = "A0998" + "T999";
		String audioCodeAQ = "A0998"+"AQ"+"L33F999";//999 
		String audioCodeAQ2 = "A0998"+"AQ"+"L25F999";
		
		String OBDCode = "V4847";
		String OBDEngRunTimeCode = OBDCode+"OBD=01 1F";
		String OBDAirTempCode = OBDCode+"OBD=01 0F";
		String OBDThrPosCode = OBDCode+"OBD=01 11";
		String OBDEngRPMCode = OBDCode+"OBD=01 0C";
		String OBDVehSpeedCode = OBDCode+"OBD=01 0D";
		String OBDCoolTempCode = OBDCode+"OBD=01 05";
		
		int clientPort = 48021;
		int serverPort = 38021;

		
		long timeRun = System.currentTimeMillis();
		long endRun = timeRun + 240000;

		
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.echo(responseTimesEcho, EchoCode, clientPort, serverPort);// κληση της echo για 4 λεπτα

		}
		
	// throughput
		echopackets.calculateThroughput(throughPut);
		
		throughPut.close();
		responseTimesEcho.close();

		
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		packetCounter = 0;
		sumResponseTimes = 0;
		responseTimes.clear();
		
		
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.echo(responseTimesEchoWithoutDelay, EchoCodeWithoutDelay, clientPort, serverPort);

		}

		// throughput
		echopackets.calculateThroughput(throughPutWithoutDelay);
		responseTimesEchoWithoutDelay.close();
		throughPutWithoutDelay.close();
		

		echopackets.temperatures(temper, EchoCode2, clientPort, serverPort);//  temperature 
																			

		temper.close();

		
		


		echopackets.image(im, imageCode, clientPort, serverPort);
		im.close();
		
		echopackets.image(im2, imageCode2, clientPort, serverPort);
		im2.close();
		 

	
		echopackets.audio(audioCode,DPCMsamplesClip,DPCMDiffFs, clientPort,  serverPort);
		
		
		
		echopackets.audioAQ(audioCodeAQ,AQDPCMDiffs1,AQDPCMsamples1,AQDPCMstepClip1,AQDPCMmeanClip1, clientPort, serverPort);
		
		echopackets.audioAQ(audioCodeAQ2,AQDPCMDiffs2,AQDPCMsamples2,AQDPCMstepClip2,AQDPCMmeanClip2, clientPort, serverPort);
		
		
	
 		echopackets.audio(audioCodeFreq,DPCMsamplesFreq,DPCMDiffFs2, clientPort,  serverPort);
 		
 		
 		
 		
 		DPCMsamplesFreq.close() ; 
		 DPCMsamplesClip.close(); 
		 AQDPCMsamples1.close() ; 
		
		 DPCMDiffFs.close() ;
		 DPCMDiffFs2.close() ;
		
		 AQDPCMDiffs1.close() ;
		
		 AQDPCMDiffs2.close() ;
		 AQDPCMsamples2.close() ; 
		
		 AQDPCMstepClip1.close() ; 
		 AQDPCMstepClip2.close() ; 
		
		 AQDPCMmeanClip1.close(); 
		 AQDPCMmeanClip2.close() ; 
 		
 		
 		
		
		
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 30000;
		
		while (System.currentTimeMillis() < endRun) {
			echopackets.ithakiCopter(ithCoptLmotor,ithCoptRmotor,ithCoptAlt,ithCoptTemp,ithCoptPress,48038,38048);
		

		}
		
		
		
		ithCoptRmotor.close();
		ithCoptLmotor.close();
		ithCoptAlt.close();
		ithCoptTemp.close();
		ithCoptPress.close();
		
		//OBD
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDEngRunTime,OBDEngRunTimeCode, clientPort,serverPort);


		}//11
		
		
		
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDAirTemp,OBDAirTempCode, clientPort,serverPort);


		}//15
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDThrPos,OBDThrPosCode, clientPort,serverPort);


		}//19
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDEngRPM,OBDEngRPMCode, clientPort,serverPort);


		}//23
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDVehSpeed,OBDVehSpeedCode, clientPort,serverPort);


		}//27
		timeRun = System.currentTimeMillis();
		endRun = timeRun + 240000;
		
		while (System.currentTimeMillis() < endRun) {

			echopackets.OBDII(OBDCoolTemp,OBDCoolTempCode, clientPort,serverPort);


		}//31
		
		
		OBDEngRunTime.close();
		OBDAirTemp.close();
		OBDThrPos.close();
		OBDEngRPM.close();
		OBDVehSpeed.close();
		OBDCoolTemp.close();
		
		
	}

	
	public void Requests(String c, int sp) throws IOException {

		DatagramSocket s = new DatagramSocket();

		String packetInfo = c;

		int serverPort = sp;

		byte[] code = packetInfo.getBytes();

		byte[] hostIP = { (byte) 155, (byte) 207, (byte) 18, (byte) 208 };

		InetAddress hostAddress = InetAddress.getByAddress(hostIP);

		DatagramPacket p = new DatagramPacket(code, code.length, hostAddress, serverPort);

		s.send(p);

		s.close();
	}



	public void echo(PrintWriter resTiEcho, String code, int clp, int sp) throws IOException {

		int clientPort = clp;

		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(3000);

		byte[] rxbuffer = new byte[32];

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		String message = new String(rxbuffer, 0, q.getLength());

		long startT;
		long endT;

		int responsetimes;

		Requests(code, sp); 

		try {

			startT = System.currentTimeMillis();// χρονος εκκινησης του προγραμματος

			r.receive(q);

			endT = System.currentTimeMillis();

			responsetimes = (int) (endT - startT); // ευρεση χρονου αποκρισης

			resTiEcho.println(responsetimes); // εγγραφη των χρονων αποκρισης σε αρχειο

			message = new String(rxbuffer, 0, q.getLength());
			packetCounter++; // αριθμηση των πακετων που λαμβανουμε
			
			responseTimes.add(responsetimes);
			
			System.out.println(message + " " + "packet No:" + packetCounter);

		} catch (Exception x) {
			System.out.println(x);
		}

		r.close();

	}

	void calculateThroughput(PrintWriter throughput) {
		
		int movingTime;
		int sumUntil16s=0;
		int morethan16s=0;
		double pcktCount=0;
		double valOfthrPut;
		
		for(int runtime=0;runtime<224000 ;runtime+=1000) {//θα τρεξει για τα 4 λεπτα που παιρνουμε πακετα,με βημα 1000(ms) ,δηλ 1 sec 
			
			movingTime=runtime;
			for(int j=0;j<responseTimes.size();j++) {//τρεχει στο μηκος του πινακα που κραταει ολα τα responsetimes
				
				//
				if(responseTimes.get(j)<movingTime && movingTime>0) {
					
					movingTime = movingTime - responseTimes.get(j);
					continue;
					
					
				}
				if(responseTimes.get(j)>movingTime && movingTime>0 ) {
					sumUntil16s = responseTimes.get(j) - movingTime;
					pcktCount = (double)sumUntil16s/(double)responseTimes.get(j);
					movingTime=0;
					continue;
					
					
				}
				
				sumUntil16s = sumUntil16s + responseTimes.get(j);
				if(sumUntil16s<=16000) {
					pcktCount++;
				}
				else {
					morethan16s = sumUntil16s - 16000;
					pcktCount+= (double) (responseTimes.get(j)-morethan16s)/(double)responseTimes.get(j);
					valOfthrPut = (double)  (pcktCount/(double)16);
					throughput.println(valOfthrPut);
					sumUntil16s=0;
					pcktCount=0;
					break;
				}
				
			}
				
		}
		
	}
	
	
	
	
	
	
	// συναρτηση για την ληψη θερμοκρασιων απο τους αισθητηρες
	public void temperatures(PrintWriter temp, String code, int clp, int sp) throws IOException {

		int clientPort = clp;

		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(2500);

		byte[] rxbuffer = new byte[55];

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		String message = new String(rxbuffer, 0, q.getLength());

		Requests(code, sp); // κληση της Request για να γινει αιτημα του client προς τον server

		try {

			r.receive(q);

			message = new String(rxbuffer, 0, q.getLength());
			packetCounter++;
			System.out.println(message + " " + "packet No:" + packetCounter);

		} catch (Exception x) {
			System.out.println(x);
		}

		String temps = new String(rxbuffer, 43, 3);

		temp.println(temps); // εγγραψη των θερμοκρασιων σε αρχειο

		r.close();

	}

	// συναρτηση για ληψη της εικονας
	public void image(FileOutputStream ima, String code, int clientPort, int sp) throws IOException {
		DatagramSocket r = new DatagramSocket(clientPort);

		byte[] rxbuffer = new byte[128];

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		boolean flag = true;
		
		boolean startwriting = false;

		while (flag) {

			Requests(code, sp); // κληση της Request για να γινει αιτημα του client προς τον server
			r.receive(q);
			for (int i = 0; i < 128; i++) {

				if (i < 127) {

				
					if((rxbuffer[i] == (byte) 0xFF && rxbuffer[i + 1] == (byte) 0xD8) || startwriting ) {
						
						if(startwriting==false) {
							System.out.println("start " + (int)rxbuffer[i]+" " + (int)rxbuffer[i + 1]);
							
						}
						
						//System.out.println("startttttttttttttttttttttttttttttttttt");
						
						startwriting = true;//ωστε να γινεται true καθε φορα η συνθηκη ελεγχου
						//System.out.println((int) rxbuffer[i]);
						ima.write(rxbuffer[i]);
						if(i == 126) {
							ima.write(rxbuffer[127]);
							
						}
					}
					
					if (rxbuffer[i] == (byte) 0xFF && rxbuffer[i + 1] == (byte) 0xD9) {
						System.out.println("endddddddddddddddddddddddddddddddddddddd");
						flag = false;
						System.out.println((int) rxbuffer[i+1]);
						
						if(i<126){
							ima.write(rxbuffer[i+1]);
						}
						
						break;//ωστε αν τα byte τελους ειναι στη μεση του πακετου,να μην γραφτει το υπολοιπο πακετο
					}
					
				
				}
				
				
			}

			
		}
		r.close();

	}
	
	
	
	public void audio(String code, PrintWriter DPCMsamples ,PrintWriter DPCMDiffFs , int clp, int sp) throws IOException, LineUnavailableException {

		int clientPort = clp;

		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(3000);

		byte[] rxbuffer = new byte[128];//132 για να καλυφθει και η περιπτωση του AQ

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		
		Requests(code, sp); // κληση της Request για να γινει αιτημα του client προς τον server
		
		byte [][] elastic_buffer = new byte [960][128];//992 πακετα ειναι 31 δευτερεολεπτα ηχου
		byte [][] samples_buffer = new byte [960][256];
		
		samples_buffer[0][0] = (byte)0;
		
		//αποκωδικοποιηση
		int splitter1 = 15;//00001111
		int splitter2 = 240;//11110000
		int nibble1 = 0;
		int nibble2 = 0;
		
		int diff1;
		int diff2;
		
		int beta = 1;
		
	
		
		
		
		//γεμισμα του elastic_buffer με τα πακετα ,οπως αυτα εληφθησαν
		for(int i=0;i<999;i++) {
			try {
			r.receive(q);
			}
			catch(Exception x) {
				System.out.println(x);
				
			}
			
			
			if(i < 960) {
				for(int j=0;j<rxbuffer.length;j++) {
				
					elastic_buffer[i][j] = rxbuffer[j];
				}
			
			}
		}
		
		
		for(int i=0;i<960;i++) {
			
			for(int j=0;j<rxbuffer.length;j++) {
				nibble2 = (splitter1 & elastic_buffer[i][j]);
				nibble1 = ((splitter2 & elastic_buffer[i][j])>>4);
				
				diff1 = (nibble1-8)*beta;
				diff2 = (nibble2-8)*beta;
				
				DPCMDiffFs.println(diff1);
				DPCMDiffFs.println(diff2);
				
				
				if(j==0) {
					samples_buffer[i][2*j] = (byte)diff1;
					samples_buffer[i][2*j +1] = (byte)(diff2 + (int)samples_buffer[i][2*j]) ;
				}
				else {
				samples_buffer[i][2*j] = (byte)(diff1 + samples_buffer[i][2*j - 1]);
				samples_buffer[i][2*j +1] = (byte)(diff2 + (int)samples_buffer[i][2*j]) ;
				}
				
			}
		}
		
		AudioFormat linearPCM = new AudioFormat(8192,8,1,true,false); 
		 
		SourceDataLine lineOut = AudioSystem.getSourceDataLine(linearPCM); 
		 
		lineOut.open(linearPCM,320000); 
		 
		lineOut.start(); 
		 
		byte[] audioBufferOut = new byte[320000]; 
		
		 
		
		
		
		for(int i=0;i<960;i++) {
			for(int j=0;j<256;j++) {
			audioBufferOut[i*256+j]=samples_buffer[i][j];
			
			DPCMsamples.println(samples_buffer[i][j]);
			
			}
		}
		
		lineOut.write(audioBufferOut,0,320000); 
		
		lineOut.stop();
		lineOut.close();
		
		
		r.close();
	}
	
	public void audioAQ(String code,PrintWriter AQDPCMDiffs,PrintWriter AQDPCMsamples,PrintWriter AQDPCMstepClip,PrintWriter AQDPCMmeanClip, int clp, int sp) throws IOException, LineUnavailableException {
		int clientPort = clp;

		
		System.out.println("hellooooo");
		
		
		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(3000);

		byte[] rxbuffer = new byte[132];//132 για να καλυφθει και η περιπτωση του AQ

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		
		Requests(code, sp); // κληση της Request για να γινει αιτημα του client προς τον server
		
		byte [][] elastic_buffer = new byte [960][132];//992 πακετα ειναι 31 δευτερεολεπτα ηχου
		byte [][] samples_buffer = new byte [960][512]; // λογικα πρεπει να ειναι 512 bytes
		samples_buffer[0][0] = (byte)0;
		
		//αποκωδικοποιηση
		
		int splitter1 = 15;
		int splitter2 = 240;
		
		int splitter3 = 255;//0000000011111111
		int splitter4 = 65280;//1111111100000000
		int nibble1 = 0;
		int nibble2 = 0;
		
		int diff1;
		int diff2;
		int meanLsb,meanMsb,mean,meanLsbSign,meanMsbSign,meanSign ;
		int stepLsb ,stepMsb,step,stepLsbSign ,stepMsbSign,stepSign;
		int sample1,sample2;
		int sample0 = 0;
		
		
		
		int count=0;
		
		//γεμισμα του elastic_buffer με τα πακετα ,οπως αυτα εληφθησαν,
		for(int i=0;i<960;i++) {
			System.out.println("hellooooo before receieve");
			try {
				r.receive(q);
				}
				catch(Exception x) {
					System.out.println(x);
					
				}
			count++;
			System.out.println("hellooooo after receieve count" + count);
			for(int j=0;j<rxbuffer.length;j++) {
			
				elastic_buffer[i][j] = rxbuffer[j];
			}
			
		}
		
		System.out.println("helloooooagainnn");
		
		
		for(int i=0;i<960;i++) {
			//περναμε στο αρχειο τις τιμες των step και mean προτου γινουν unsigned
			meanLsbSign = (int)(elastic_buffer[i][0]);
			meanMsbSign = (int)(elastic_buffer[i][1]);
			stepLsbSign = (int)(elastic_buffer[i][2]);
			stepMsbSign = (int)(elastic_buffer[i][3]);
			
			meanMsbSign = (meanMsbSign<<8);
			meanSign = (meanMsbSign | meanLsbSign);
			
			
			stepMsbSign = (stepMsbSign<<8);
			stepSign = (stepMsbSign | stepLsbSign);
			
			
			
			AQDPCMmeanClip.println(meanSign);
			
			//unsigned
			meanLsb = (int)(elastic_buffer[i][0] & 0xFF);
			meanMsb = (int)(elastic_buffer[i][1] & 0xFF);
			stepLsb = (int)(elastic_buffer[i][2] & 0xFF);
			stepMsb = (int)(elastic_buffer[i][3] & 0xFF);
			
			
			
			
			meanMsb = (meanMsb<<8);
			mean = (meanMsb | meanLsb);
			
			
			stepMsb = (stepMsb<<8);
			step = (stepMsb | stepLsb);
			AQDPCMstepClip.println(step);
			
			System.out.println("kai edooooo");
			
			for(int j=4;j<rxbuffer.length;j++) {
				nibble2 = (splitter1 & elastic_buffer[i][j]);
				nibble1 = ((splitter2 & elastic_buffer[i][j])>>4);
				
				diff1 = (nibble1-8);
				diff2 = (nibble2-8);
				
				AQDPCMDiffs.println(diff1);
				AQDPCMDiffs.println(diff2);
				
				//σε καθε μεταβλητη βαζω 16 bits τα οποια πρεπει να διαχωριστουν σε 8 + 8 για να μπουν σε 1 + 1 byte
				
				sample1 = step*diff1 + mean; 
				sample2 = step*diff2 + mean;
				
				
				
				//στην πρωτη θεση μπαινει το LSBYTE
				
				samples_buffer[i][4*(j - 4)] =  (byte) (splitter3 & sample1);
				samples_buffer[i][4*(j - 4) + 1] = (byte) ((splitter4 & sample1)>>8);
				samples_buffer[i][4*(j - 4) + 2] =(byte) (splitter3 & sample2); 
				samples_buffer[i][4*(j - 4) + 3] = (byte) ((splitter4 & sample2)>>8);
				
				
				
				
				
			}
		}
		
		
		AudioFormat linearPCM = new AudioFormat(8000,16,1,true,false); 
		 
		SourceDataLine lineOut = AudioSystem.getSourceDataLine(linearPCM); 
		 
		lineOut.open(linearPCM,600000); 
		 
		lineOut.start(); 
		 
		byte[] audioBufferOut = new byte[600000]; 
		
		 
		
		
		
		for(int i=0;i<960;i++) {
			for(int j=0;j<512;j++) {
			audioBufferOut[i*512+j]=samples_buffer[i][j];
			AQDPCMsamples.println(samples_buffer[i][j]);
			}
		}
		
		lineOut.write(audioBufferOut,0,600000); 
		
		lineOut.stop();
		lineOut.close();
		
		
		r.close();
		
	}
	
	void ithakiCopter (PrintWriter lmotor,PrintWriter rmotor,PrintWriter alt,PrintWriter temp,PrintWriter press, int clp, int sp) throws IOException {
		
		int clientPort = clp;

		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(3000);

		byte[] rxbuffer = new byte[128];//132 για να καλυφθει και η περιπτωση του AQ

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		
		
		String message = new String(rxbuffer, 0, q.getLength());
		
		try {
			
			r.receive(q);

			message = new String(rxbuffer, 0, q.getLength());
			packetCounter++; // αριθμηση των πακετων που λαμβανουμε
			System.out.println(message + " " + "packet No:" + packetCounter);

			
			lmotor.println(message.substring(40,43));
			rmotor.println(message.substring(51,54));
			alt.println(message.substring(64,67));
			temp.println(message.substring(80,86));
			press.println(message.substring(96,103));
			
		} catch (Exception x) {
			System.out.println(x);
		}
		
		
		

		r.close();
	
	
	
	
	}
	void OBDII (PrintWriter file,String obd, int clp,int sp ) throws IOException {
		
		int clientPort = clp;

		DatagramSocket r = new DatagramSocket(clientPort);
		r.setSoTimeout(3000);

		byte[] rxbuffer = new byte[128];//132 για να καλυφθει και η περιπτωση του AQ

		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		
		
		String message = new String(rxbuffer, 0, q.getLength());
		Requests(obd , sp);
		
		try {
			
			r.receive(q);

			message = new String(rxbuffer, 0, q.getLength());
			packetCounter++; // αριθμηση των πακετων που λαμβανουμε
			System.out.println(message + " " + "packet No:" + packetCounter + obd);
			System.out.println(obd.substring(12,14));
			System.out.println(obd.substring(12,14).length());
			int runTime;
			int airTemp;
			int throPos;
			int engRpm;
			int vehiSp;
			int CoolTemp;
			
			if(obd.substring(12,14).equals("1F")) {
				System.out.println(message.substring(9,11));
				runTime =	(256* (Integer.parseInt(message.substring(6,8),16)) + Integer.parseInt(message.substring(9,11),16));
				System.out.println(runTime);
				file.println(runTime);
			}
			
			else if(obd.substring(12,14).equals("0F")) {
				airTemp = (int) ( Integer.parseInt(message.substring(6,8),16) - 40);
				file.println(airTemp);
				
			}
			
			else if(obd.substring(12,14).equals("11")) {
				throPos = (int) ( (Integer.parseInt(message.substring(6,8),16)*100)/255);
				file.println(throPos);
				
			}
			else if(obd.substring(12,14).equals("0C")) {
				engRpm = (256* (Integer.parseInt(message.substring(6,8),16)) + Integer.parseInt(message.substring(9,11),16))/4;
				file.println(engRpm);
				
			}
			else if(obd.substring(12,14).equals("0D")) {
				vehiSp = Integer.parseInt(message.substring(6,8),16);
				file.println(vehiSp);
				
			}
			else if(obd.substring(12,14).equals("05")) {
				airTemp = (int) ( Integer.parseInt(message.substring(6,8),16) - 40);
				file.println(airTemp);
				
			}
			
			
			
		} catch (Exception x) {
			System.out.println(x);
		}
		
		
		

		r.close();
	
		
		
	}
	
	
	
	

}
