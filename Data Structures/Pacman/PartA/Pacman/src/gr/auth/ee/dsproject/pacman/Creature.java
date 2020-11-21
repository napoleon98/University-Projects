/* �������-������ ������ ���:9124 gpappasv@ece.auth.gr 6970650963
 * �������� �����������  ���:9170 napoleop@ece.auth.gr 6974331167
  
 */


package gr.auth.ee.dsproject.pacman;

/**
 * <p>
 * Title: DataStructures2006
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

  public int calculateNextPacmanPosition (Room[][] Maze, int[] currPosition)
  {

    int newDirection = -1;

    while (newDirection == -1) {

      int temp_dir = (int) (4 * Math.random());

      if (Maze[currPosition[0]][currPosition[1]].walls[temp_dir] == 1) {
        newDirection = temp_dir;
      }

    }
    step++;

    return newDirection;
  }

  // THIS IS THE FUNCTION TO IMPLEMENT!!!!!!
  //��������� ��� ��� ��������� ��� ������� �������� ��� �����������.
  public int[] calculateNextGhostPosition (Room[][] Maze, int[][] currentPos)
  {

	  int [] ghostsArray = new int[PacmanUtilities.numberOfGhosts];// ���������� ������ �� ������ ���� ��� � ������� � �����������.
	 //������ ��� ������ ghostsArray �� ����� ��� ������� for.
	  for(int i=0;i<PacmanUtilities.numberOfGhosts;i++) {
		  ghostsArray[i] = (int)( 4* Math.random()); //������� ��� ������ �� ������� ����� ��� 0-3
	  }
	  //������� for ��� ��� ������ ��� ����������� ��� �������� ��� ��������� � ������� ghostsArray.
	  for(int i=0;i<PacmanUtilities.numberOfGhosts;i++) {
		  int x= currentPos[i][0];// ���������� ���������� ��� ��� ���������� ��� ����� ��� ������ ������ ��� ������ currentPos ��� �������� ��� ������������� ��� ����� ��� ���� �����������.
		  int y = currentPos[i][1];// ���������� ���������� ��� ��� ���������� ��� ����� ��� �������� ������ ��� ������ currentPos ��� �������� ��� ������������� ��� ����� ��� ���� �����������.
		   boolean [] arr = checkCollision(ghostsArray, currentPos);// ����������  ������ ����� boolean ��� ��� ���������� ��� ������ ��� ���������� � ��������� checkCollision.
			int p = Maze[x][y].walls[ghostsArray[i]];// ���������� ���������� ����� �������� ��� ��� ���������� ��� ����� ���  Maze[x][y].walls[ghostsArray[i]] 
			 // ������� while ��� ������ ��� � ������ ��� ����������� ��� ����� ������. 
			while(p==0 || arr[i]) {
				  ghostsArray[i] = (int)( 4* Math.random()); // ������� ����� ������� ����� ��� 0-3
				  p = Maze[x][y].walls[ghostsArray[i]];// ��������� ��� ����� ��� ���������� p ���� �� ��� ����� �������� � �������.
				   arr = checkCollision(ghostsArray, currentPos);//  ��������� ��� ����� ��� ���������� arr ���� �� ��� ����� �������� � �������.
			  }
			
			
		  
	  }
  
	  	return ghostsArray;// ��������� ��� ������ �� ��� ������� �������� ��� �����������
  
  }

  public boolean[] checkCollision (int[] moves, int[][] currentPos)
  {
    boolean[] collision = new boolean[PacmanUtilities.numberOfGhosts];

    int[][] newPos = new int[4][2];

    for (int i = 0; i < moves.length; i++) {

      if (moves[i] == 0) {
        newPos[i][0] = currentPos[i][0];
        newPos[i][1] = currentPos[i][1] - 1;
      } else if (moves[i] == 1) {
        newPos[i][0] = currentPos[i][0] + 1;
        newPos[i][1] = currentPos[i][1];
      } else if (moves[i] == 2) {
        newPos[i][0] = currentPos[i][0];
        newPos[i][1] = currentPos[i][1] + 1;
      } else {
        newPos[i][0] = currentPos[i][0] - 1;
        newPos[i][1] = currentPos[i][1];
      }

      collision[i] = false;
    }

    for (int k = 0; k < moves.length; k++) {
      // System.out.println("Ghost " + k + " new Position is (" + newPos[k][0] + "," + newPos[k][1] + ").");
    }

    for (int i = 0; i < moves.length; i++) {
      for (int j = i + 1; j < moves.length; j++) {
        if (newPos[i][0] == newPos[j][0] && newPos[i][1] == newPos[j][1]) {
          // System.out.println("Ghosts " + i + " and " + j + " are colliding");
          collision[j] = true;
        }

        if (newPos[i][0] == currentPos[j][0] && newPos[i][1] == currentPos[j][1] && newPos[j][0] == currentPos[i][0] && newPos[j][1] == currentPos[i][1]) {
          // System.out.println("Ghosts " + i + " and " + j + " are colliding");
          collision[j] = true;
        }

      }

    }
    return collision;
  }

}
