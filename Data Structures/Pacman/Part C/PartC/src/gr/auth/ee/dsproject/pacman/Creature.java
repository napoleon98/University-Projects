/* �������-������ ������ ���:9124 gpappasv@ece.auth.gr 6970650963
 * �������� �����������  ���:9170 napoleop@ece.auth.gr 6974331167
  
 */

package gr.auth.ee.dsproject.pacman;

import java.util.ArrayList;

import gr.auth.ee.dsproject.node.Node91709124;

/**
 * <p>
 * Title: DataStructures2011
 * </p>
 * 
 * <p>
 * Description: Data Structures project: year 2011-2012
 * </p>
 * 
 * <p>
 * Copyright: Copyright (c) 2011
 * </p>
 * 
 * <p>
 * Company: A.U.Th.
 * </p>
 * 
 * @author Michael T. Tsapanos
 * @version 1.0
 */

public class Creature implements gr.auth.ee.dsproject.pacman.AbstractCreature
{

  public String getName ()
  {
    return "Mine";
  }

  private int step = 1;
  private boolean amPrey;

  public Creature (boolean isPrey)
  {
    amPrey = isPrey;

  }
  
  // ���������� ���������� ������� 
  
// ��������� ��� ���������� ��� ������� ������ ��� pacman
  public int calculateNextPacmanPosition (Room[][] Maze, int[] currPosition)
  { 
    Node91709124 root = new Node91709124(-1,currPosition,0,null,Maze); // ���������� ��� ����� ��� �������
    createSubTreePacman(root.getDepth()+1, root, Maze, currPosition);  // ����� ��� ���������� createSubTreePacman ��� ��� ���������� ��� ������� ��� ����� ��� �������. 
	
    double mostSutMove = ABprun(root,2,Double.NEGATIVE_INFINITY,Double.POSITIVE_INFINITY,true); //����� ��� ���������� ABprun �� �������� root, ����� ������� 2 �=-�� ��� b=+oo ���������� ��� �� max part (maxP=true)
   
    int moveToReturn=0; // ������ ��� ������������ �� 0 ��� ���������� moveToReturn ��� �� �������� �������� ��� ���������� ���  �������� ������� ��� pacman
       
    //������ ������� for ��� ��� ����������� ��� node ��� �������� �� evaluation ��� ����������� ���� ���� ��� mostSutMove 
    for(int i=0;i<root.getChildren().size();i++) {
    	for(int y=0;y<root.getChildren().get(i).getChildren().size();y++) {
    		
    		if (mostSutMove==root.getChildren().get(i).getChildren().get(y).getnodeEvaluation())
    			{moveToReturn=root.getChildren().get(i).getChildren().get(y).getNodeMove();
    		}
    	}
    	
    	
    }
       
    
    
    return moveToReturn;// ��������� ��� ������� �������
    
    
    
  }
  
    

  double ABprun(Node91709124 node,int treeDepth,double a, double b, boolean maxP)//ab pruning
	{
	  if (treeDepth == 0 || node.getChildren().isEmpty())//������� �� �� ����� ������� ����� ����� � �� ��� ���� ������ �� node ��� ����� �� ������ ���� ���������
		  return node.getnodeEvaluation();//��������� ��� evaluation ��� node �� ��������� ��� ������ � �������� �������
	  
	  if (maxP) {//������� �� � ������� ��� max � ��� min ����� ��� �������
		  double v=Double.NEGATIVE_INFINITY;//������� ���� ���� ��� v �� -��
		  
		  for(int i=0;i<node.getChildren().size();i++) {//������� for ��� ����� ��� ��� ������ ��� ������� ��� node
			  
			if(v<ABprun(node.getChildren().get(i),treeDepth-1,a,b,false))//������� �� � ��������� � ���� ��������� ���� ��� ��� ��������� ��� ���������� ABprun ��� ����� ���� ��� ��������� ��� �� ����� ��� ������ ����
				v=ABprun(node.getChildren().get(i),treeDepth-1,a,b,false);//������� ��� ����� ��� ���������� ��� �������� ������ ���� � �� ������ � �������
			
			if(v>a)a=v;// ������� �� �� � �������� ���������� ���� ��� �� a ��� ������� ��� ����� ��� � ��� a �� ������ � �������
				if(b<=a)break;//�� �� b<=a ������� ������� ��� ������� ���
				
		  }
		  return v;//��������� ��� ����� ��� �
		  
	  }
	  else //�� � ABprun ������� ��� ������� ��� ������ �� ��������� �� �������� evaluation �������� �� ��������
	  {
		  double v=Double.POSITIVE_INFINITY;//������� ��� ����� +oo ���� �
		  
		  for(int i=0;i<node.getChildren().size();i++) {//������� for ��� ����� ��� ��� ������ ��� ������� ��� node
			  
			if(v>ABprun(node.getChildren().get(i),treeDepth-1,a,b,false))//������� �� � ��������� � ���� ���������� ���� ��� ��� ��������� ��� ���������� ABprun ��� ����� ���� ��� ��������� ��� �� ����� ��� ������ ����
				v=ABprun(node.getChildren().get(i),treeDepth-1,a,b,false);//������� ��� ����� ��� ���������� ��� �������� ������ ���� � �� ������ � �������
			
			if(v<b)b=v;// ������� �� �� � �������� ���������� ���� ��� �� b ��� ������� ��� ����� ��� � ��� b �� ������ � �������
				if(b<=a)break;//�� �� b<=a ������� ������� ��� ������� ���
				
		  }
		  return v;//��������� ��� ����� ��� �
		  
	  }
		  
		  
		  
	  
	} 
  
  void createSubTreePacman (int depth, Node91709124 parent, Room[][] Maze, int[] currPacmanPosition)
  {


		int moveToReturn=(int)(Math.random()*4);//������ ������������ ��� ���������� ��� �� ������� ��� ������� ������ ��� ������ �� ��������� ��� ���� �� ������� �������� ���������� �������� (������ �� ��������� ��� ��� ������������� �� ����������
		
		
		
		ArrayList<Node91709124> avpos=new ArrayList <Node91709124>();//���������� arraylist ������������ ��� �� ������������ �� ����������� ����� Node91709124 ���� �� ��������� ������ ��� ��������� ���������
		
		Node91709124[] obj=new Node91709124[4];//������� ������������ ����� Node91709124 �������� ������
		obj[0]=new Node91709124(0,currPacmanPosition,depth,null,Maze);//����������� Node91709124 �� ������ ���� ��� ���������� ��� ������� �� 0
		obj[1]=new Node91709124(1,currPacmanPosition,depth,null,Maze);//����������� Node91709124 �� ������ ���� ��� ���������� ��� ������� �� 1
		obj[2]=new Node91709124(2,currPacmanPosition,depth,null,Maze);//����������� Node91709124 �� ������ ���� ��� ���������� ��� ������� �� 2
		obj[3]=new Node91709124(3,currPacmanPosition,depth,null,Maze);//����������� Node91709124 �� ������ ���� ��� ���������� ��� ������� �� 3
		
		for(int i=0;i<4;i++){//for ��� ������ ��� ����� ��� 3 ���� �� ��������� ���� ��� ������� ������������
			if(Maze[currPacmanPosition[0]][currPacmanPosition[1]].walls[i]==1)//������� �� ���� ���������� ��� ������� � ���� ��� i ������� ������
				if(Maze[obj[i].getNodeX()][obj[i].getNodeY()].isGhost()==false)//������� �� ���� ���� ��� ��������� �� ���� � ������ (��� ������� � ���� ��� i) ��������� �� ������ �������� (���� �� ���������� �� ��������� ��� ������������ ������ �� ��������)
					avpos.add(obj[i]);//�� ���� ���������� ��� ������� �� i ��� ��������� �� ������ ��������, ������������ �� ���������� ����������� �� ��� ��������� ���� ����������� ���� arraylist ����� �������� ��� ������ ������ ��� ������. ����� ����������� ��� ���� ��� ������������� �������� ������������ ������ ���� arraylist, �� ������������ ����� �� ������������ ��������
			
		}
		
		
		
	
		
		
		Room[][][] copies = new Room[avpos.size()][][]; // ������ ������������� ������� ���� �������� ���� ��������� ��� Maze ��� ��� �� ���������� �������� ��� pacman.�� ����, ���� ��� ������ ��������� ������������ �� ���������� ������ ��� maze
		
		
		int [] nextPositions = new int[2]; // ������ ������ �������� 2 ���� �� ��������� �� ������������� ��� �������� ������� ��� pacman
		
		//������� for ��� ������ ��� 0 ����� ��� ������ ��� ���������� �������� ��� pacman
		for(int i=0;i<avpos.size();i++) {
			
			 copies[i] = PacmanUtilities.copy(Maze); // ���������� ��� ���������� ��� maze ���� i ���� ��� ������ ��� ����������
			 nextPositions = PacmanUtilities.evaluateNextPosition(Maze,currPacmanPosition,avpos.get(i).getNodeMove(),PacmanUtilities.borders);//���������� ���� ������ nextPositions ��� �������� ����� ��� pacman ���� ��� ������ �� ����� ��� ���������� evaluateNextPosition
			 Node91709124 test = new Node91709124(avpos.get(i).getNodeMove(),currPacmanPosition,0,null,Maze);//���������� ������������ ����� Node91709124 �� ����� test
			 PacmanUtilities.movePacman(copies[i],currPacmanPosition ,nextPositions); //��������� ��� ������� ��� pacman �� ����� ��� ���������� movePacman
			 Node91709124 child = new  Node91709124 (avpos.get(i).getNodeMove(),currPacmanPosition,parent.getDepth() +1 ,parent, copies[i]);// ���������� ���� node,��� ����� ��� �����-node ��� ���� ��� ������ ��� ���������� createSubTreePacman
			child.setnodeEvaluation(test.getnodeEvaluation());//���������� ��� ���� ��� evaluate ��� child �� ���� ��� test
			child.setnodeXY(test.getNodeX(),test.getNodeY());//���������� ��� ����� nodeX nodeY ��� child �� ����� ��� test
			 parent.setChildren(child); // ���������� ��� child ��� arraylist ��� parent �� ����� ��� ����������� setter 
			 createSubTreeGhosts (child.getDepth()+1,child,copies[i],child.getCurrentGhostsPos()); 	//����� ��� ���������� 	createSubTreeGhosts ��� �� ���������� ��� ���������� ��� �����������
	
		}
		
		
		
		
		

  }
 
  void createSubTreeGhosts (int depth, Node91709124 parent, Room[][] Maze, int[][] currGhostsPosition)
  {
	 
	  
	  ArrayList<int[][]> ghostAvMoves = new ArrayList<int[][]>();//������ arraylist ���� �� ������������ �������� ���� �� ������� �������� ��� �����������
	 
	  ghostAvMoves = PacmanUtilities.allGhostMoves(Maze,currGhostsPosition);//������� ��� ���������� ����� ��� arraylist ghostAvMoves �� ����� ��� ���������� PacmanUtilities.allGhostMoves
	  int[][] prevGhostPos=currGhostsPosition;//���������� ��� ������ ���� ��� ����������� ��� ����� ��������
	  Room[][][] copies = new Room[ ghostAvMoves.size()][][];//���������� ������ ����� room ���� �� ������������ �� ��������� ��� maze
	  
	  for(int i =0; i<ghostAvMoves.size();i++) {//for ��� 0 ��� ��� ������ ��� ������� �������� ��� ����������� ��� ��������� ��� �� size ��� �������� arraylist
			 copies[i]=PacmanUtilities.copy (Maze);//���������� ���������� ��� ���������� ��� ���v i ���� ��� copies
			 
			 PacmanUtilities.moveGhosts(copies[i],currGhostsPosition,ghostAvMoves.get(i));//����������� ��� ������� ��� ����������� �� ���� ��� ������� �������� ��� ������ � ���� i ��� arraylist			 
			  int[] pacmanPos= new int [2];
			  //������� ���� ������ pacmanPos ��� ������������� ��� ������ ���� ��������� ��� maze ��� ������ �� ������ ���� ��� ����� ��� createSubTreeGhost
			  pacmanPos[0]=parent.getNodeX();
			  pacmanPos[1]=parent.getNodeY();
			 Node91709124 newNode  = new  Node91709124(parent.getNodeMove(),pacmanPos,depth,parent,copies[i]);//���������� ��� ���� node �� ���� ��� �������� �������� ��� �����������
			 newNode.setnodeEvaluation(0);//����������� ��� ���� ��� evaluation ��� �������� node
			 parent.setChildren(newNode);//������� �� newNode �� ����� ��� parent (��� ������������ ��� ����� ������ ��� ������ ���� ��� ����� ����� ��� ����������)
			 int ghostCounterCloser=0;// ��������� ��� ������� ��� ������ ��� ����������� ��� ��������� ��� ������
			 //������� ��������� ��� �� ���� �������� �� � ������ ��� ����� �������� �������� �� ��� ����� ��� ����� ���� ������
			 if(Math.sqrt((prevGhostPos[0][0]-pacmanPos[0])*(prevGhostPos[0][0]-pacmanPos[0])+(prevGhostPos[0][1]-pacmanPos[1])*(prevGhostPos[0][1]-pacmanPos[1]))>(currGhostsPosition[0][0]-pacmanPos[0])*(currGhostsPosition[0][0]-pacmanPos[0])+(currGhostsPosition[0][1]-pacmanPos[1])*(currGhostsPosition[0][1]-pacmanPos[1]))ghostCounterCloser++;
			 if(Math.sqrt((prevGhostPos[1][0]-pacmanPos[0])*(prevGhostPos[1][0]-pacmanPos[0])+(prevGhostPos[1][1]-pacmanPos[1])*(prevGhostPos[1][1]-pacmanPos[1]))>(currGhostsPosition[1][0]-pacmanPos[0])*(currGhostsPosition[1][0]-pacmanPos[0])+(currGhostsPosition[1][1]-pacmanPos[1])*(currGhostsPosition[1][1]-pacmanPos[1]))ghostCounterCloser++;
			 if(Math.sqrt((prevGhostPos[2][0]-pacmanPos[0])*(prevGhostPos[2][0]-pacmanPos[0])+(prevGhostPos[2][1]-pacmanPos[1])*(prevGhostPos[2][1]-pacmanPos[1]))>(currGhostsPosition[2][0]-pacmanPos[0])*(currGhostsPosition[2][0]-pacmanPos[0])+(currGhostsPosition[2][1]-pacmanPos[1])*(currGhostsPosition[2][1]-pacmanPos[1]))ghostCounterCloser++;
			 if(Math.sqrt((prevGhostPos[3][0]-pacmanPos[0])*(prevGhostPos[3][0]-pacmanPos[0])+(prevGhostPos[3][1]-pacmanPos[1])*(prevGhostPos[3][1]-pacmanPos[1]))>(currGhostsPosition[3][0]-pacmanPos[0])*(currGhostsPosition[3][0]-pacmanPos[0])+(currGhostsPosition[3][1]-pacmanPos[1])*(currGhostsPosition[3][1]-pacmanPos[1]))ghostCounterCloser++;
			 //�� ��� �������� if ������ ���� ���������� ��������� ��� ������ ��� ������� ��� ���� ��� evaluation ��� newNode �� ���� ���� ��� �� nodeEvaluation ��� parent
			 newNode.setnodeEvaluation(ghostCounterCloser*(-12)+parent.getnodeEvaluation());
			 		 
			 
			 			 
		 } 
		  
		  
		  
		  
		  
	  }
	  
	 
  
  
  
  
  

  public int[] getPacPos (Room[][] Maze)
  {
    int[] pacmanPos = new int[2];
    for (int i = 0; i < PacmanUtilities.numberOfRows; i++) {
      for (int j = 0; j < PacmanUtilities.numberOfColumns; j++) {
        if (Maze[i][j].isPacman()) {
          pacmanPos[0] = i;
          pacmanPos[1] = j;
          return pacmanPos;
        }
      }
    }
    return pacmanPos;
  }

  public boolean[] comAvPos (Room[][] Maze, int[][] currentPos, int[] moves, int currentGhost)
  {

    boolean[] availablePositions = { true, true, true, true };

    int[][] newPos = new int[4][2];

    for (int i = 0; i < 4; i++) {

      if (Maze[currentPos[currentGhost][0]][currentPos[currentGhost][1]].walls[i] == 0) {
        availablePositions[i] = false;
        continue;
      }

      if (PacmanUtilities.flagColision(Maze, currentPos[currentGhost], i)) {
        availablePositions[i] = false;
      }

      else if (currentGhost == 0)
        continue;

      else {
        switch (i) {
        case Room.WEST:
          newPos[currentGhost][0] = currentPos[currentGhost][0];
          newPos[currentGhost][1] = currentPos[currentGhost][1] - 1;
          break;
        case Room.SOUTH:
          newPos[currentGhost][0] = currentPos[currentGhost][0] + 1;
          newPos[currentGhost][1] = currentPos[currentGhost][1];
          break;
        case Room.EAST:
          newPos[currentGhost][0] = currentPos[currentGhost][0];
          newPos[currentGhost][1] = currentPos[currentGhost][1] + 1;
          break;
        case Room.NORTH:
          newPos[currentGhost][0] = currentPos[currentGhost][0] - 1;
          newPos[currentGhost][1] = currentPos[currentGhost][1];

        }

        for (int j = (currentGhost - 1); j > -1; j--) {
          switch (moves[j]) {
          case Room.WEST:
            newPos[j][0] = currentPos[j][0];
            newPos[j][1] = currentPos[j][1] - 1;
            break;
          case Room.SOUTH:
            newPos[j][0] = currentPos[j][0] + 1;
            newPos[j][1] = currentPos[j][1];
            break;
          case Room.EAST:
            newPos[j][0] = currentPos[j][0];
            newPos[j][1] = currentPos[j][1] + 1;
            break;
          case Room.NORTH:
            newPos[j][0] = currentPos[j][0] - 1;
            newPos[j][1] = currentPos[j][1];
            // break;
          }

          if ((newPos[currentGhost][0] == newPos[j][0]) && (newPos[currentGhost][1] == newPos[j][1])) {

            availablePositions[i] = false;
            continue;
          }

          if ((newPos[currentGhost][0] == currentPos[j][0]) && (newPos[currentGhost][1] == currentPos[j][1]) && (newPos[j][0] == currentPos[currentGhost][0])
              && (newPos[j][1] == currentPos[currentGhost][1])) {

            availablePositions[i] = false;

          }
        }
      }
    }

    return availablePositions;
  }

  public int comBestPos (boolean[] availablePositions, int[] pacmanPosition, int[] currentPos)
  {

    int[] newVerticalDifference = new int[2];
    for (int i = 0; i < 2; i++)
      newVerticalDifference[i] = currentPos[i] - pacmanPosition[i];

    int[] distanceSquared = new int[4];

    for (int i = 0; i < 4; i++) {
      if (availablePositions[i] == true) {

        switch (i) {
        case Room.WEST:
          newVerticalDifference[1]--;
          break;
        case Room.SOUTH:
          newVerticalDifference[0]++;
          break;
        case Room.EAST:
          newVerticalDifference[1]++;
          break;
        case Room.NORTH:
          newVerticalDifference[0]--;
          break;
        }
        distanceSquared[i] = newVerticalDifference[0] * newVerticalDifference[0] + newVerticalDifference[1] * newVerticalDifference[1];
      } else
        distanceSquared[i] = PacmanUtilities.numberOfRows * PacmanUtilities.numberOfRows + PacmanUtilities.numberOfColumns * PacmanUtilities.numberOfColumns + 1;
    }

    int minDistance = distanceSquared[0];
    int minPosition = 0;

    for (int i = 1; i < 4; i++) {
      if (minDistance > distanceSquared[i]) {
        minDistance = distanceSquared[i];
        minPosition = i;
      }

    }

    return minPosition;
  }

  public int[] calculateNextGhostPosition (Room[][] Maze, int[][] currentPos)
  {

    int[] moves = new int[PacmanUtilities.numberOfGhosts];

    int[] pacmanPosition = new int[2];

    pacmanPosition = getPacPos(Maze);
    for (int i = 0; i < PacmanUtilities.numberOfGhosts; i++) {
      moves[i] = comBestPos(comAvPos(Maze, currentPos, moves, i), pacmanPosition, currentPos[i]);
    }

    return moves;

  }

  public boolean[] checkCollision (int[] moves, int[][] currentPos)
  {
    boolean[] collision = new boolean[PacmanUtilities.numberOfGhosts];

    int[][] newPos = new int[4][2];

    for (int i = 0; i < moves.length; i++) {

      if (moves[i] == 0) {
        if (currentPos[i][1] > 0) {
          newPos[i][0] = currentPos[i][0];
          newPos[i][1] = currentPos[i][1] - 1;
        } else {
          newPos[i][0] = currentPos[i][0];
          newPos[i][1] = PacmanUtilities.numberOfColumns - 1;
        }

      } else if (moves[i] == 1) {
        if (currentPos[i][0] < PacmanUtilities.numberOfRows - 1) {
          newPos[i][0] = currentPos[i][0] + 1;
          newPos[i][1] = currentPos[i][1];
        } else {
          newPos[i][0] = 0;
          newPos[i][1] = currentPos[i][1];
        }
      } else if (moves[i] == 2) {
        if (currentPos[i][1] < PacmanUtilities.numberOfColumns - 1) {
          newPos[i][0] = currentPos[i][0];
          newPos[i][1] = currentPos[i][1] + 1;
        } else {
          newPos[i][0] = currentPos[i][0];
          newPos[i][1] = 0;

        }
      } else {
        if (currentPos[i][0] > 0) {
          newPos[i][0] = currentPos[i][0] - 1;
          newPos[i][1] = currentPos[i][1];
        } else {

          newPos[i][0] = PacmanUtilities.numberOfRows - 1;
          newPos[i][1] = currentPos[i][1];

        }
      }

      collision[i] = false;
    }

    for (int k = 0; k < moves.length; k++) {

    }

    for (int i = 0; i < moves.length; i++) {
      for (int j = i + 1; j < moves.length; j++) {
        if (newPos[i][0] == newPos[j][0] && newPos[i][1] == newPos[j][1]) {

          collision[j] = true;
        }

        if (newPos[i][0] == currentPos[j][0] && newPos[i][1] == currentPos[j][1] && newPos[j][0] == currentPos[i][0] && newPos[j][1] == currentPos[i][1]) {

          collision[j] = true;
        }

      }

    }
    return collision;
  }

}
