# include "chess.h"
# include <stdlib.h>

int Board::check_validity(int id)
{
		pos p = get_position(id);
		if(p.x == -1)
		{
				return 0;
		}
		else
		{
				return 1;
		}
}	

void Board::draw()
{
		int i = 0, j =0;

		//
		// Draw the Board.
		//

		for(i = 0; i < 9; i ++)
		{
				for(j = 0; j < 9; j++)
				{
						if(matrix[i][j] != NULL)
						{
								if((matrix[i][j]->getId() / 10) == 0)
								{
										cout << " |  " << matrix[i][j]->getId() << " ";
								}	
								else
								{
										cout <<" | " << matrix[i][j]->getId() <<" ";
								}	
						}		
						else
						{
								cout <<" |    "; 
						}	
				}
				cout << " |" <<endl;
		}

		//
		// Print the Orientation of the Pieces.
		//

		cout << "Pieces :: \n";

		for(i = 1; i < 37; i++)
		{
				if(Piece_array[i] != NULL)
				{
						Piece_array[i]->print_orientation(i);
						cout << "\n";
				}
				else
				{
						cout << i << " 0\t"; 
				}
				if (i%4 == 0)	
				{
						cout << "\n";
				}

		}		

}

pos Board::get_position(int id)
{
		int i = 0, j = 0;
		pos p;
		p.x = -1;
		p.y = -1;

		for(i = 0; i < 9; i ++)
		{
				for(j = 0; j < 9; j++)
				{
						if(matrix[i][j] != NULL)
						{
								if(matrix[i][j]->getId() == id)
								{
										p.x = i;
										p.y = j;
										return p;
								}
						}
				}	
		}

		return p;
}

/*
 *  Moves the Piece in the specified direction
 *  Returns 1 if the move is invalid else returns 0
 */
int Board::make_move(int id, int mov_dir)
{
		pos p = get_position(id);
		int i = p.x;
		int j = p.y;

		switch(mov_dir)	
		{
				case 1:
						if(i > 0)
						{
								i--;
						}
						else
						{
								return 1;
						}
						break;
				case 2:
						if(i < 8)
						{
								i++;
						}
						else
						{
								return 1;
						}
						break;
				case 3:
						if(j < 8)
						{
								j++;
						}
						else
						{
								return 1;
						}
						break;
				case 4:
						if(j > 0 )
						{
								j--;
						}
						else
						{
								return 1;
						}

		}

		if((id == 35 || id == 36) && matrix[i][j] != NULL)
		{
				//
				// Hypercube move. 
				//
				int t_x;
				int t_y;

				srand(time(NULL));

				do
				{
						t_x = rand()%9;	
						t_y = rand()%9;
				}while(matrix[t_x][t_y] != NULL);

				matrix[t_x][t_y] = matrix[i][j];
		}
		else
		{
				if(matrix[i][j] != NULL)
				{
						//
						// Some piece will die. Remove it from the Piece_array as well
						//
						Piece_array[matrix[i][j]->getId()] = NULL;

				}
		}
		matrix[i][j] = matrix[p.x][p.y];
		matrix[p.x][p.y] = NULL;

		return 0;
}

int Board::rotate(int id)
{
		pos p = get_position(id);			
		matrix[p.x][p.y]->change_orientation();
		if(id == 33 || id == 35)
		{
				return 1;
		}
		else
		{
				return 0;
		}

}

void Board::action(int id)
{
		//
		// Only Valid for the Gun piece.
		//

		Gun * g = (Gun *) Piece_array[id];
		g -> shoot(this, id);
}

/*
 * beam function shoots a beam on the Board.
 * i is ith row and j is jth column at which the beam currently is
 * dir = 1 => beam will travel northwards
 * dir = 2 => beam will travel southwards
 * dir = 3 => beam will travel eastwards
 * dir = 4 => beam will travel westwards
 * */
void Board::beam(int i, int j, int dir)
{
		while(1)
		{
				switch(dir)
				{
						case 1:
								i--;
								break;
						case 2:
								i++;
								break;
						case 3:
								j++;
								break;
						case 4:
								j--;			
				}

				if( (i >= 0 && i < 9) && (j >= 0 && j < 9) )
				{
						if(matrix[i][j] != NULL)
						{
								if(can_kill(i, j, dir))
								{
										return;
								}

						}	
				}
				else
				{
						return;
				}		
		}
}

/*
 * can_kill function of class Board takes the row i and column j of the Board
 * and checks if the piece at the current position can be killed.
 * A piece cannot be killed it its a mirror with its face towards the beam.
 * dir gives the direction of the beam
 * */
int Board::can_kill(int i, int j, int &dir)
{
		int id = matrix[i][j]->getId();
		int ref;

		if(id >= 1 && id <=12)
		{
				//
				// Triangular mirror. Checking for reflection.
				//


				Triangle * t = (Triangle *) matrix[i][j];
				ref = t->reflect(dir);

				if(ref != -1)
				{
						//
						// Beam has to be reflected in the direction ref.
						//

						dir = ref;
						return 0;

				}	
		}
		else if(id >= 13 && id <= 20)
		{
				//
				// Square mirror. Checking for reflection.
				//

				Square * s = (Square *) matrix[i][j];
				ref = s->reflect(dir);

				if(ref != -1)
				{
						//
						// Beam has to be reflected in the direction ref.
						//

						dir = ref;
						return 0;

				}	
		}
		else if(id == 21 || id == 22 || id == 25 || id ==26)
		{
				//
				// Slant mirror. Find reflection.
				//

				Slantline * s = (Slantline *) matrix[i][j];
				ref = s->reflect(dir);
				dir = ref;
				return 0;			
		}
		else if(id == 23 || id == 24 || id == 27 || id == 28)
		{
				//
				// Horizontal or Vertical mirror. Find reflection.
				//

				Line * l = (Line *) matrix[i][j];
				ref = l->reflect(dir);
				dir = ref;
				return 0;			
		}
		else if(id == 29 || id == 30)
		{
				//
				// Beam Splitter. Find reflection.
				//

				Splitter * s = (Splitter *) matrix[i][j];
				ref = s->reflect(this, i, j, dir);
				dir = ref;
				return 0;			
		}
		else if(id == 35 || id == 36)
		{
				//
				// Hypercube. Kill Nothing.
				//

				return 0;
		}

		kill(i, j);
		return 1;
}

/*
 * kill function of class Board take the row i and column j of the Board
 * and kills the piece at that position
 * */
void Board::kill(int i, int j)
{
		Piece_array[matrix[i][j]->getId()] = NULL;
		matrix[i][j] = NULL;	

}

int Board::stateDifference(Board b2, int team)
{
		int i =0;
		int value1 = 0, value2 = 0, sum_r_s1 = 0, sum_r_s2 = 0;
		int sum_g_s1 = 0;
		int sum_g_s2 = 0;

		for(i = 1; i < 37; i++)
		{
				if(Piece_array[i] != NULL)
				{
						if(Piece_array[i]->getTeam() == 1)
								sum_r_s1 += Piece_array[i]->getValue();
				}
		}

		for(i = 1; i < 37; i++)
		{
				if(b2.Piece_array[i] != NULL)
				{
						if(b2.Piece_array[i]->getTeam() == 1)
								sum_r_s2 += b2.Piece_array[i]->getValue();
				}
		}


		for(i = 1; i < 37; i++)
		{
				if(Piece_array[i] != NULL)
				{
						if(Piece_array[i]->getTeam() == 0)
								sum_g_s1 += Piece_array[i]->getValue();
				}
		}

		for(i = 1; i < 37; i++)
		{
				if(b2.Piece_array[i] != NULL)
				{
						if(b2.Piece_array[i]->getTeam() == 0)
								sum_g_s2 += b2.Piece_array[i]->getValue();
				}
		}

		value2 = sum_g_s2 - sum_g_s1;


		//
		// Compute Net Profit (Profit + Loss)
		//

		return (value1 + value2);
}

Board Board::computeMoves(int level)
{
		int i;
		int cost[89];	

		Board new_state[89];

		for(i = 0; i < 89; i++ )
		{
				//			new_state[i] = new Board();
				//				cout << i << "\n";
				new_state[i].initialize(this);
		}

		//
		// Evaluate all the Green moves.
		//

		int best_move = computeGreenMoves(new_state, level, cost);

		cout << best_move<<"\n";
		//new_state[best_move].draw();
		return new_state[best_move];
}


int Board::computeGreenMoves(Board new_state[89], int level, int cost[89])
{
		if(level == 1)
		{

				//
				// Compute all the possible moves for triangles
				//

				computePieceMove(7, 12, 0, cost, new_state, 0);
				//
				// Square Pieces.
				//

				computePieceMove(17, 20, 30, cost, new_state, 0);

				//
				// Slant Line and Line 
				//

				computePieceMove(25, 28, 50, cost, new_state, 0);

				//
				// Splitter  
				//

				computePieceMove(30, 30, 70, cost, new_state, 0);

				//
				// King
				//

				computePieceMove(32, 32, 75, cost, new_state, 0);

				//
				// Gun 
				//

				computePieceMove(34, 34, 79, cost, new_state, 0);

				//
				// The  following state represents the firing of Laser.
				//

				if(!new_state[84].check_validity(34))
				{
						cost[84] = -999;
				}
				else
				{
						new_state[84].action(34);
						cost[84] = stateDifference(new_state[84], 0);
				}

				//
				// Hypercube
				//

				computePieceMove(36, 36, 85, cost, new_state, 0);

				//
				// Return the best state.
				//

		}
		else
		{
				//
				// Compute Red moves for each Green move.
				//

				//
				// Compute all the possible moves for triangles
				//

				computeNextLevelMoves(7, 12, 0, 1, level - 1, new_state, cost);

				//
				// Square Pieces.
				//

				computeNextLevelMoves(17, 20, 30, 1, level - 1, new_state, cost);

				//
				// Slant Line and Line 
				//

				computeNextLevelMoves(25, 28, 50, 1, level - 1, new_state, cost);

				//
				// Splitter  
				//

				computeNextLevelMoves(30, 30, 70, 1, level - 1, new_state, cost);

				//
				// King
				//

				computeNextLevelMoves(32, 32, 75, 1, level - 1, new_state, cost);

				//
				// Gun 
				//

				computeNextLevelMoves(34, 34, 79, 1, level - 1, new_state, cost);

				//
				// The  following state represents the firing of Laser.
				//

				if(!new_state[84].check_validity(34))
				{
						//
						// No further computation. Invalid state.
						//
						cost[84] = -999;
				}
				else
				{
						new_state[84].action(34);
						int v = stateDifference(new_state[84], 1);
						if(v < 0)	
						{
							//
							// Killing your own piece. Invalid Move. Prune.
							//
										
										cost[84] = -999;
						}
						else
						{
						int l;

						Board new_state1[89];
						int cost1[89];
						int state;

						for(l = 0; l < 89; l++ )
						{	
								new_state1[l].initialize(&new_state[84]);
						}

						state = new_state[84].computeRedMoves(new_state1, level - 1, cost1);
						cost[84] = cost1[state];
						}
				}



				//
				// Hypercube
				//

				computeNextLevelMoves(36, 36, 85, 1, level - 1, new_state, cost);

		}

		int max = -999;
		int state = -1;
		int count = 0;

		for(int i = 0; i < 89; i++)
		{
				if(cost[i] != -999 && cost[i] > max)
				{
						max = cost[i];
						state = i;
						count = 1;
				}
				else if(cost[i] == max)
				{
						count++;
				}
		}


		if(count > 1)
		{
				//
				// Randomization
				//

				state = find_cpu_state(state, max, count, cost);
		}	

		return state;
}

int Board::computeRedMoves(Board new_state[89], int level, int cost[89])
{
		if(level == 1)
		{

				//
				// Compute all the possible moves for triangles
				//

				computePieceMove(1, 6, 0, cost, new_state, 1);

				//
				// Square Pieces.
				//

				computePieceMove(13, 16, 30, cost, new_state, 1);

				//
				// Slant Line and Line 
				//

				computePieceMove(21, 24, 50, cost, new_state, 1);

				//
				// Splitter  
				//

				computePieceMove(29, 29, 70, cost, new_state, 1);

				//
				// King
				//

				computePieceMove(31, 31, 75, cost, new_state, 1);

				//
				// Gun 
				//

				computePieceMove(33, 33, 79, cost, new_state, 1);

				//
				// The  following state represents the firing of Laser.
				//

				if(!new_state[84].check_validity(33))
				{
						cost[84] = -999;
				}
				else
				{
						new_state[84].action(33);
						cost[84] = stateDifference(new_state[84], 1);
				}

				//
				// Hypercube
				//

				computePieceMove(35, 35, 85, cost, new_state, 1);

				//
				// Return the best state.
				//

		}
		else
		{
				//
				// Compute Green moves for all Red moves.
				//

				//
				// Compute all the possible moves for triangles
				//

				computeNextLevelMoves(1, 6, 0, 0, level - 1, new_state, cost);

				//
				// Square Pieces.
				//

				computeNextLevelMoves(13, 16, 30, 0, level - 1, new_state, cost);

				//
				// Slant Line and Line 
				//

				computeNextLevelMoves(21, 24, 50, 0, level - 1, new_state, cost);

				//
				// Splitter  
				//

				computeNextLevelMoves(29, 29, 70, 0, level - 1, new_state, cost);

				//
				// King
				//

				computeNextLevelMoves(31, 31, 75, 0, level - 1, new_state, cost);

				//
				// Gun 
				//

				computeNextLevelMoves(33, 33, 79, 0, level - 1, new_state, cost);

				//
				// The  following state represents the firing of Laser.
				//

				if(!new_state[84].check_validity(33))
				{
						//
						// No further computation. Invalid state.
						//
						cost[84] = -999;
				}
				else
				{
						new_state[84].action(33);
						int v = stateDifference(new_state[84], 1);
						if(v > 0)	
						{
							//
							// Killing your own piece. Invalid Move. Prune.
							//
										
										cost[84] = -999;
						}
						else
						{
						int l;

						Board new_state1[89];
						int cost1[89];
						int state;

						for(l = 0; l < 89; l++ )
						{	
								new_state1[l].initialize(&new_state[84]);
								//								new_state1[l] = new Board(&new_state[84]);
						}


						state = new_state[84].computeGreenMoves(new_state1, level - 1, cost1);
						cost[84] = cost1[state];
						}
				}



				//
				// Hypercube
				//

				computeNextLevelMoves(35, 35, 85, 0, level - 1, new_state, cost);

		}

		int min = 999;
		int state = -1;
		int count = 0;

		for(int i = 0; i < 89; i++)
		{
				if(cost[i] != -999 && cost[i] <  min)
				{
						min = cost[i];
						state = i;
						count = 1;
				}
				else if(cost[i] == min)
				{
						count++;
				}
		}


		if(count > 1)
		{
				//
				// Randomization
				//

				state = find_cpu_state(state, min, count, cost);
		}	

		return state;
}

		void 
Board::computeNextLevelMoves(int id1, int id2, int j, int turn, int level, Board new_state[89], int cost[89])
{
		int i;

		for(i = id1; i <= id2; i++)	
		{
				if(!new_state[j].check_validity(i))
				{
						//
						// No further computation. Invalid state.
						//

						//
						// Piece doesn't exist
						//
						int m;
						int n;
						int p;

						m = j;

						if(i == 32 || i == 36 || i == 31 || i == 35)
						{
								//
								// Hypercube | King :: Only 4 moves
								//

								n = j + 4;	
						}
						else
						{
								//
								// 5 moves for rest of the Pieces.
								//

								n = j + 5;
						}	

						//
						// All moves will be invalid
						//

						for(p = 0; m < n; m++, p++, j++)
						{
								cost[j] = -999;
						}	
				}
				else
				{
						int m;
						int n;
						int p;

						m = j;
						n = j + 4;

						//
						// Perform all the moves.
						//

						for(p = 1; m < n; m++, p++, j++)
						{
								//
								// p gives the move direction.
								//
								int invalid = 0;

								invalid = new_state[j].make_move(i, p);
								int v = stateDifference(new_state[j], 1);
								if((turn == 1 && v < 0) || (turn == 0 && v > 0) || invalid)	
								{
										//
										// Killing your own piece. Invalid Move. Prune.
										//
										
										cost[j] = -999;
								}
								else
								{
										int l;

										Board new_state1[89];
										int cost1[89];
										int state;

										for(l = 0; l < 89; l++ )
										{	
												new_state1[l].initialize(&new_state[j]);
										}

										if(turn)
										{
												state = new_state[j].computeRedMoves(new_state1, level, cost1);
										}
										else
										{
												state = new_state[j].computeGreenMoves(new_state1, level, cost1);

										}

										cost[j] = cost1[state];
								}
						}


						if(i != 32 && i != 36 && i != 31 && i!= 35)
						{

								//
								// Not applicable to King and Hypercube.
								//

								new_state[j].rotate(i);

								int l;

								Board new_state1[89];
								int cost1[89];
								int state;

								for(l = 0; l < 89; l++ )
								{	
										new_state1[l].initialize(&new_state[j]);
								}

								if(turn)
								{
										state = new_state[j].computeRedMoves(new_state1, level, cost1);
								}
								else
								{
										state = new_state[j].computeGreenMoves(new_state1, level, cost1);		
								}

								cost[j] = cost1[state];
								j++;
						}	
				}


				//
				// Perform the rotate move.
				//
		}
}


		void 
Board::computePieceMove(int id1, int id2, int j, int cost[89], Board new_state[89], int team)
{
		int i;

		for(i = id1; i <= id2; i++)	
		{
				if(!new_state[j].check_validity(i))
				{
						//
						// Piece doesn't exist
						//
						int m;
						int n;
						int p;

						m = j;

						if(i == 32 || i == 36 || i == 31 || i == 35)
						{
								//
								// Hypercube | King :: Only 4 moves
								//

								n = j + 4;	
						}
						else
						{
								//
								// 5 moves for rest of the Pieces.
								//

								n = j + 5;
						}	

						//
						// All moves will be invalid
						//

						for(p = 0; m < n; m++, p++, j++)
						{
								cost[j] = -999;
						}	
				}
				else
				{
						int m;
						int n;
						int p;

						m = j;
						n = j + 4;

						//
						// Perform all the moves.
						//

						for(p = 1; m < n; m++, p++, j++)
						{
								//
								// p gives the move direction.
								//

								int invalid = 0;

								invalid = new_state[j].make_move(i, p);
								if(invalid)
								{
										cost[j] = -999;
								}
								else
								{
										cost[j] = stateDifference(new_state[j], team);
								}

						}


						//
						// Perform the rotate move.
						//

						if(i != 32 && i != 36 && i != 35 && i!= 31)
						{

								//
								// Not applicable to King and Hypercube.
								//

								new_state[j].rotate(i);
								cost[j] = stateDifference(new_state[j], team);
								j++;
						}	
				}

		}

}

		int 
Board::find_cpu_state (int state, int max, int count, int cost[89])
{
		int rand_state[count];

		for(int i = 0, j = 0; i < 89; i++)
		{
				if(cost[i] == max)
				{
						rand_state[j] = i; 
						j++;
				}
		}

		srand(time(NULL));
		return rand_state[rand()%count];

}

