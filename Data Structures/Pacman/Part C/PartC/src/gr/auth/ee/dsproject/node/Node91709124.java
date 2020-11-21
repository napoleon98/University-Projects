/* �������-������ ������ ���:9124 gpappasv@ece.auth.gr 6970650963
 * �������� �����������  ���:9170 napoleop@ece.auth.gr 6974331167
  
 */
package gr.auth.ee.dsproject.node;

import gr.auth.ee.dsproject.pacman.PacmanUtilities;
import gr.auth.ee.dsproject.pacman.Room;

import java.util.ArrayList;

public class Node91709124 
{

  int nodeX;
  int nodeY;
  int depth;
  int nodeMove;
  double nodeEvaluation;
  Node91709124  parent;
  ArrayList<Node91709124 > children = new ArrayList<Node91709124 >();

  int[][] currentGhostPos=new int[4][2];
  int[][] flagPos= new int[4][2];
  boolean[] currentFlagStatus=new boolean[4];
  public int getNodeX(){return nodeX;}
  public int getNodeY(){return nodeY;}
  public void setNodeMove(int i){nodeMove=i;}
  public int getNodeMove(){return nodeMove;}
  public double getnodeEvaluation(){return nodeEvaluation;}
  public int getDepth() {return depth;}
  public Node91709124 getParent() { return parent;}
  public ArrayList<Node91709124 > getChildren() {return children;}
  public void setChildren(Node91709124 Child) { children.add(Child);}
  public int[][] getCurrentGhostsPos(){return currentGhostPos;}
  public void setnodeEvaluation(double e) {nodeEvaluation=e;}
  public void setnodeXY(int xi,int yi) {nodeX=xi;
  nodeY=yi;}
  
  
//��������� ������� �������� ��� ��� ������������ �� ��������� ����� ���� ��� ����������.
  public Node91709124  (Room[][] Maze)
  {
	  nodeX=0;
	  nodeY=0;
	  nodeMove=0;
	  nodeEvaluation=0;
	  currentGhostPos=findGhosts(Maze);
	  flagPos=findFlags(Maze);
	  currentFlagStatus=checkFlags(Maze);
	  depth=0;
	  parent = null;
	 
  }
//��������� ������� �������� �� ��������� ��������(1 ������ ��� ���� ��������� ��� ����� ����� �� ����) ��� ��� ������������ ��� ���������� �� ��� ����� ��� ����� � �������.
  public Node91709124 (int direction,  int[] currPosition,int dpth,Node91709124 prt  , Room[][] Maze) // Constructor
  {
	 
	  int [] nextPositions = new int [2]; 
	  nextPositions=PacmanUtilities.evaluateNextPosition(Maze,currPosition,  direction, PacmanUtilities.borders);
	  nodeX = nextPositions[0];
	  nodeY = nextPositions[1];
    nodeMove=direction;
    parent = prt;
    depth = dpth;
    currentGhostPos=findGhosts(Maze);
    flagPos=findFlags(Maze);
    currentFlagStatus=checkFlags(Maze);
   nodeEvaluation=evaluate(direction,currPosition,Maze);
  } 

//��������� ��� ��� ������ ��� ������ ��� ����������� .
  private int[][] findGhosts (Room[][] Maze)
  {
  
 int rowghostPos=0;// ������ ����������  ���� �� ���������� � ������� ��� ������ ��� ������ ��� � ��������� ���������� ��� ����������� ��� �� 0.
    int[][] ghostPos =new int[4][2];// ������ ��� ����������� ������ �� ��� ������ �� ����������� ��� �� ���������� � ���������.
   // ��� ������� for ��� �� ���������� ���� ��� Maze
    for(int i=0;i< PacmanUtilities.numberOfRows;i++) {
     for(int j=0;j< PacmanUtilities.numberOfColumns; j++) {
       // ������� ��� ����� ��� ���������� � ��������� isGhost() ��� ��������� ����� boolean isGhost.
      if (Maze[i][j].isGhost())// ������� ��� ���������� isGhost ��� �� ���� ���� 1 �� ������� �������� ���� ���������� ����.
      {
    	  ghostPos[rowghostPos][0]=i;//���������� ��� ���������� � ���� ����� ����� ��� ����������� ������ ��� ������ ����������.
    	  ghostPos[rowghostPos][1]=j;//���������� ��� ���������� � ��� ������� ����� ��� ����������� ������ ��� ������ ����������.
       rowghostPos++;// ������ ��� ���������� ��� �������� ��� ������ ��� ������ ��� ������ ����������.
      
      }
      }
    }
    return ghostPos; //��������� ��� ������ �� ��� ������ ��� ����������� ��� Maze.
  }

  private int[][] findFlags (Room[][] Maze)//��������� ��� ������� ��� ������������� ��� ��� ������� ��� Maze
  {

 int rowflagPos=0;//��������� ��� �� ������ ���� ������ flagPos
    int[][] flagPos =new int[4][2];//������� 4 ��� 2 (4 ������� ��� 2 ������, ��� ��� ��� x ��� ��� ��� ��� y ��� �������� �������)
    for(int i=0;i<PacmanUtilities.numberOfRows;i++) {//�������� �� 2 for �� ���� ��� Maze
     for(int j=0;j<PacmanUtilities.numberOfColumns; j++) {
      
      if (Maze[i][j].isFlag())//��������� �� ���� �������� ���� ������� ������ �� ��� ��������� isFlag
      {
        flagPos[rowflagPos][0]=i;//���� ������ ������ �� ������ ������ i,j ��� maze ������������ ��� i,j ���� ������ flagPos
        flagPos[rowflagPos][1]=j;
        rowflagPos++;//������ ��� rowflagPos ���� ���� ��� ��������� ������ ���� �� ��������� ��� ������ ���� ������ flagPos ��� �� ������������� �� �������� ��� ��� ��� ������
      
      }
      }
    }
    return flagPos;//��������� ��� ������ flagPos �� ��� ������������� ��� ���� �������
  }

  private boolean[] checkFlags (Room[][] Maze)//��������� ��� ������� �� � �������� ������ ��� maze ���� ��������� ��� ��� ������
  {
   int rowStatFlags=0;//��������� ��� �� ������ ���� ������ statFlags ��� �� ������� ���� ���� ��� ��������� ������ ���� maze
   boolean statFlags [] = new boolean[4];//������� boolean ��� �� ����������� ��� ��������� ��� ���� ������� ��� ��������� ���� ��� maze
   for(int i=0;i<PacmanUtilities.numberOfRows;i++) {//for ��� ������ �� ��� �� maze
      for(int j=0;j<PacmanUtilities.numberOfColumns; j++) {
       if(Maze[i][j].isFlag()==true)//�� ������� ������ ��� i,j//if ��� ������� �� ���� ���� i,j ��� maze ������� ������
    	 {statFlags[rowStatFlags] = Maze[i][j].isCapturedFlag();//���������� ���� ���� ��� ������� � rwoStatFlags ���� ������ ��� ���������� ��� ������� �� ����� ��� isCapturedFlag
         rowStatFlags++;}
       }
      }
   
   return statFlags;//��������� ��� ������ ���������� ��� �������
  }
  
  private double evaluate (int move,int[] currentPos,Room[][] Maze)//��������� ����������� ��������
  {

    double evaluation = 0;// ����������� ��� ���� ��� �����������
  
    //-------------�������� 1: �������� ��� �������
    
    // �� ��������� ��� � ������� ������ ��� pacman �� ��� ���� ����� �� �����, ��������� 5 ������� ��� ��� ���� ��� evaluation, ����� �� ����������� � pacman.
    if(nodeX==0 || nodeY==0 || nodeX==PacmanUtilities.numberOfRows || nodeY==PacmanUtilities.numberOfColumns) evaluation=evaluation-5;
    // �� ��������� ��� � ������� ������ ����� ��� pacman �� ��� �����, ��������� �������� 10 ������� ��� ��� ���� ��� evaluation.
    if((nodeX==0 && nodeY==0) || (nodeX==PacmanUtilities.numberOfRows && nodeY==PacmanUtilities.numberOfColumns)||(nodeX==PacmanUtilities.numberOfRows && nodeY==0)||(nodeX==0 && nodeY==PacmanUtilities.numberOfColumns)) evaluation=evaluation-10;
   
   
    //-----------------------------�������� 2 �� ���� ��� �������� ��� pacman ��� ��� �������
    int availableflags=0;//��������� ��� �� �������� ����� ������� ����� ���������� ����� ��� ��������
    boolean[] curFlSt= new boolean[4];//������� ��� �� ����������� ������� ���� ������� ������ ��� ������ ��� ��������� ��� ���� ������� ��� �������� �� ����� ��� checkFlags
    		curFlSt=checkFlags(Maze);
    for(int i=0;i<4;i++){//for � ����� ������ ���� ������ currentFlagStatus ��� ������� ���� ��� availableflags ����� ������� ����� ����� ������� ��� ��������
    	if (curFlSt[i]==false)availableflags++;//� ��������� availableflags �� ��������� ���� ������ ��� �� ��� ������ ��� ������� �������
    	 	
    	
    }
    

  
    
    double[] flagDistanceFromCurPos=new double[availableflags];//������� �������� ���� �� ��� ������� ������� ���� �� ����������� ��� �������� ��� pacman ��� ���� ������ ���� ������� ���� ��� ���������� ��� �����������
    double[] flagDistanceFromNextPos=new double[availableflags];//������� �������� ���� �� ��� ������� ������� ���� �� ����������� ��� �������� ��� pacman ��� ���� ������ ���� ������� ���� ��� ���������� ��� �����������
    int[][] flagP=new int[4][2];//������� ��� �� ����������� ���� ������ ������� ������ ��� ������ ��� ������������� ��� ���� ������� ��� ��� ���������� ��� ���������� ��� ������������
    flagP=findFlags(Maze);
    for(int i=0,k=0;i<4;i++){
    	if(!curFlSt[i]){//��������� �� � ������ ���� ���� i ��� currentFlagStatus ����� ������ ���� �� ������������� ��� �������� ��� ���� ��� ��� ���� ��� ������� ��� pacman ����� �������� �������
    		flagDistanceFromCurPos[k]=(Math.sqrt((flagP[i][0]-currentPos[0])*(flagP[i][0]-currentPos[0])+(flagP[i][1]-currentPos[1])*(flagP[i][1]-currentPos[1])));
    		flagDistanceFromNextPos[k]=(Math.sqrt((flagP[i][0]-nodeX)*(flagP[i][0]-nodeX)+(flagP[i][1]-nodeY)*(flagP[i][1]-nodeY)));
    		k++;}//�������� k ��� ������� ���� ���� ��� ��������� ������ ������ (�� ������ ����� ��� ���� availableflags, ��� ������ ��� �� ������� ��� ������� ��� ����������� ��� ����������)
    }
    
    int minDistance=0;//��������� �������� ��� �� ����������� �������� ��� ���� ��� ������ flagDistanceFromCurPos �� ��� �������� ���� (������ �������� ���� ���� ��� ������ ���������� ���� ����������� ������ ���� pacman, ���� �������)
    if (availableflags>0){//�� � ���� ��� ������� ������� ����� ����� �� ������ �������� �� ���� ������� ��� ������������� ���� ��� ������������� ��� ������� ������� ���� �� ������ ��� ������ ���������� ��� ���������� ��� �� ���� ��� �� ���� ��� ������� ��� ������
    for(int i=0;i<availableflags;i++){
    	//������ ��� ����� ��� flagDistanceFromCurPos �� ��� �������� ���� ���������
    	if(flagDistanceFromCurPos[minDistance]>flagDistanceFromCurPos[i])minDistance=i;
       	
    	}
    
    if(flagDistanceFromCurPos[minDistance]<flagDistanceFromNextPos[minDistance]) evaluation=evaluation-17;//�� � pacman ������������� ��� ��� ����������� �� ����� ������,���� ��� ������ ��� �����������, ���� ��������� 17 ������� ��� �� evaluation
    if(flagDistanceFromCurPos[minDistance]>flagDistanceFromNextPos[minDistance]) evaluation=evaluation+38; //�� � pacman �������� ��� ����������� �� ����� ������,���� ��� ������ ��� �����������, ���� ����������� 38 ������� ��� evaluation
    }
    //--------------------------------����� ��������� 2
    
    //--------------------------------�������� 3: �������� ��� �� ����������
   
    double[] ghostDistanceFromCurPos=new double[4];//������� ��� �� ����������� ��� �������� ��� pacman ��� ���� �������� ���� �� �������
    double[] ghostDistanceFromNextPos=new double[4];// ������� ��� �� ����������� ��� �������� ��� pacman ��� ���� �������� ���� �������
    int[][] currGhostPos=findGhosts(Maze);//������� ��� ���������� ��� ������������� ��� ���� �����������
    for(int i=0;i<4;i++){
    	//����������� ��� ��������� ��� pacman ��� ���� �������� ���� ��� ���� ��� ������� ��� ���� ��� ���������� ��� �����������
    	ghostDistanceFromCurPos[i]=Math.sqrt((currGhostPos[i][0]-currentPos[0])*(currGhostPos[i][0]-currentPos[0])+(currGhostPos[i][1]-currentPos[1])*(currGhostPos[i][1]-currentPos[1]));
    	ghostDistanceFromNextPos[i]=Math.sqrt((currGhostPos[i][0]-nodeX)*(currGhostPos[i][0]-nodeX)+(currGhostPos[i][1]-nodeY)*(currGhostPos[i][1]-nodeY));
    	
    }
    
    int minDistance1=0;
    
    for(int i=0;i<4;i++){
    	//������ ��� ����� ��� ghostDistanceFromCurPos �� ��� �������� ���� ���������
    	if(ghostDistanceFromCurPos[minDistance1]>ghostDistanceFromCurPos[i])minDistance1=i;
    	}
    
    
    	
    
    
    
    if(ghostDistanceFromCurPos[minDistance1]<ghostDistanceFromNextPos[minDistance1]) evaluation=evaluation+47;//�� � pacman ������������� ��� �� ����������� ��� ��������, ���� +47 ���� evaluation
    if(ghostDistanceFromCurPos[minDistance1]>ghostDistanceFromNextPos[minDistance1]) evaluation=evaluation-20;//�� � pacman �������� �� ����������� ��� ��������, ���� -20 ���� evaluation

    
    
    //---------------------------------����� ��������� 3
    
    //
    // -------------------���� ��������� 4
    if(availableflags>0)
    	//��� �������� ��� � �������� ��� ������ ��� ��� ����������� �� ����� ������ ����� ��������� ��� ��� �������� ������ ��� ������������ ����������� �� ����� ��� ��� ������������ �� ����� �������
    	if(ghostDistanceFromNextPos[minDistance1]>flagDistanceFromNextPos[minDistance] && (flagDistanceFromCurPos[minDistance]>flagDistanceFromNextPos[minDistance]))
    		{evaluation=evaluation+40;			
    		}
    //�� ��� if ��� ������� 226 ������� �� ����� �� �� �������� � ������ �� ������ ��� ������ ��� ����� ����� ��� ���� �� �������� ��� ������. �� �� +40 ��� ����������� ���� evaluation ���������� "����������" ��� ���� ��� ��������� ��� ��������� ��� �� �������� ��� ������� ���������� ���� ���� �������� ��� �������
    
    
    	return evaluation;//��������� ��� ������� ����� ����������� ��� �������

  }

}
