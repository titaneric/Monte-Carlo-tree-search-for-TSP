
// Evaluate the delta after applying a 2-opt move (delta >0 indicates an improving solution)
Distance_Type Get_2Opt_Delta(int First_City, int Second_City)
{
	if(Check_If_Two_City_Same_Or_Adjacent(First_City, Second_City)==true)
		return -Inf_Cost;
	
	int First_Next_City=All_Node[First_City].Next_City;
	int Second_Next_City=All_Node[Second_City].Next_City;
	
	Distance_Type Delta=Get_Distance(First_City,First_Next_City)+Get_Distance(Second_City,Second_Next_City)
			-Get_Distance(First_City,Second_City)-Get_Distance(First_Next_City,Second_Next_City);
	
	//Update the Chosen_Times[][] and Total_Simulation_Times which are used in MCTS			
	Chosen_Times[First_City][Second_City] ++;
	Chosen_Times[Second_City][First_City] ++;		
	Chosen_Times[First_Next_City][Second_Next_City] ++;
	Chosen_Times[Second_Next_City][First_Next_City] ++;	
	Total_Simulation_Times++;		
				
	return Delta; 
}

// Apply a chosen 2-opt move
void Apply_2Opt_Move(int First_City,int Second_City)
{
	Distance_Type Before_Distance = Get_Solution_Total_Distance();	
	Distance_Type Delta = Get_2Opt_Delta(First_City, Second_City);
	
	int First_Next_City=All_Node[First_City].Next_City;
	int Second_Next_City=All_Node[Second_City].Next_City;
	
	Reverse_Sub_Path(First_Next_City,Second_City);	
	All_Node[First_City].Next_City=Second_City;
	All_Node[Second_City].Pre_City=First_City;
	All_Node[First_Next_City].Next_City=Second_Next_City;	
	All_Node[Second_Next_City].Pre_City=First_Next_City;
	
	// Update the values of matrix Weight[][] by back propagation, which would be used in MCTS
	double Increase_Rate=Beta*(pow(2.718, (double) (Delta) /(double)(Before_Distance)) -1);
	
	Weight[First_City][Second_City] += Increase_Rate;
	Weight[Second_City][First_City] += Increase_Rate;		
	Weight[First_Next_City][Second_Next_City] += Increase_Rate;
	Weight[Second_Next_City][First_Next_City] += Increase_Rate;		
}


bool Improve_By_SA_Move()
{
	bool If_Improved=false;	
	double alpha = 0.99;
	double temperature_min = 0.0001;
	double temperature = 1;
	int cur_node = Get_Random_Int(Virtual_City_Num);

	while (temperature > temperature_min) {

		for(int j=0;j<Candidate_Num[cur_node];j++)	
		{			
			int Candidate_City=Candidate[cur_node][j];
			Distance_Type delta = Get_2Opt_Delta(cur_node,Candidate_City);
			double rand_num = ((double) rand() / (RAND_MAX));
			if(delta>0)
			{
				Apply_2Opt_Move(cur_node,Candidate_City);			
				If_Improved=true;
				break;					
			}
			else if (rand_num < pow(2.718, (double) (-delta) / (temperature)))
			{
				cur_node = Candidate_City;
			}
		}
		
		temperature *= alpha;	
	}
	return If_Improved;
}

// Iteratively apply an improving 2-opt move until no improvement is possible
void Local_Search_by_SA_Move()
{
	while(Improve_By_SA_Move()==true)
		;	
	// Improve_By_SA_Move();
	// cout << "out\n";
		
	Distance_Type Cur_Solution_Total_Distance=Get_Solution_Total_Distance();		
	if(Cur_Solution_Total_Distance < Current_Instance_Best_Distance)
	{
		// Store the information of the best found solution to Struct_Node *Best_All_Node
		Current_Instance_Best_Distance = Cur_Solution_Total_Distance;	
		Store_Best_Solution();	  		
	}	
} 
