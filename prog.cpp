#include <stdio.h>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <fstream>

#define n 9			//number of rows in the memory
#define m 9				//number of columns in the memory
using namespace std;
void printPMT(map<int,map<int,vector<int> > >);
void printMemory(int [][n]);
void printFree(int []);
void printAlloc(int []);
void printbl_pages(pair<int,int> []);
void printMatrix(int [][3]);
void printFinalMap(map<int, pair<int,vector<int> > >);
int main()
{
	int p; 					// p represents the total number of processes entering
	int free[m*n];		//list of free blocks present in the memory, this will contain (i,j) from which we can calculate which block is free.
	int alloc[m*n];		//list of allocated blocks in the memory, this will also contain (i,j) as a pair
	int memory[n][m];
	int memory1[n][m];
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			memory1[i][j] = 0;
		}
	}
	int pages = 0;
	int count1 = 0;
	int count2 = 0;
	int total_pages = n*n;
	int block_size = n;
	map<int, map<int,vector<int> > > pmt;
	pair<int,int> bl_pages[n];			//It maps a number of pages to block id.
	int matrix[100000][3];				//This is the copy of page map table which is used to find number of allocated blocks.
	vector<pair<int,int> > cur;			//vector of pairs which will be used to put values in our main map
	map<int,vector<pair<int,int> > > finalmap;
	ofstream myfile1;
	ofstream myfile2;
	int pagesArray[10000];				//to store number of pages corresponding to each process.
	int blockArray[10000];				//to keep track of allocated blocks.
	for(int i=0;i<10000;i++)
	{
		blockArray[i] = 0;
	}
    myfile1.open ("example1.txt");
    myfile2.open ("example2.txt");
	for(int i=0;i<n;i++)
	{
		bl_pages[i].second = i;
		bl_pages[i].first = 0;
	}
	int k = 0;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			memory[i][j] = 0;		// 0 represents that the current page in the memory is free. We will fill the page by P_ID when it will not be 							free
			
			//initially all the pages in the memory are free, so free list contains all the pairs <i,j>
			free[k] = 1;
			alloc[k] = 0;
			k++;
		}
	}
	printf("Enter the total number of queries\n");
	scanf("%d",&p);
	for(int i=0;i<100000;i++)
	{
		for(int j=0;j<3;j++)
		{
			matrix[i][j] = -1;
		}
	}
	while(p > 0)
	{	
		vector<string> fields;
		printf("Enter P_ID, s/f and #pages\n");
		while(getchar()!=10);
		int p_id,num_pages;
		char c;
		char s[100];
		scanf("%[^\n]",s);
		string buf;
		stringstream ss(s);
		vector<string> tokens;
		while(ss >> buf)
		{
			tokens.push_back(buf);
		}
		p_id = atoi(tokens[0].c_str());
		if(tokens[1] == "s")
		{
			num_pages = atoi(tokens[2].c_str());
			pages += num_pages;
			int ix = 0;
			map<int,vector<int> > temp;
			vector<int> pv(2);
			pagesArray[p_id] = num_pages;
	//		pmt.insert(p_id,NULL);
			/** This while loop will put a page of a process in the memory depending upon which block in the memory is empty, If some block in the memory
			* is not empty then it will again search for some block till it finds some.
			*/
			while(ix < num_pages)
			{
				//printf("hello\n");
				int puti = rand()%n;
				int putj = rand()%m;
				if(memory[puti][putj] == 0)
				{
					memory[puti][putj] = p_id;
					int mem_page = (puti*n) + putj;		//generating the block number using x and y co-ordinate of the memory,
					pv[0] = mem_page;
					int block = (puti/(int)sqrt(n))*(int)sqrt(n) + putj/(int)sqrt(n);
					pv[1] = block;
					
					for(int i =0;i<n;i++)
					{
						if(bl_pages[i].second == block)
						{
							bl_pages[i].first++;
						}
					}
					free[mem_page] = 0;
					alloc[mem_page] = 1;
					/*for(int i=0;i<m*n;i++)
					{
						if(free[mem_page] == 1)
						{
							free[mem_page] = 0;
							alloc[mem_page] = 1;
							break;
						}
					}*/
					temp.insert(pair<int,vector<int> >(ix,pv));
					ix++;
				}
			}			
			pmt.insert(pair<int,map<int,vector<int> > >(p_id,temp));
			printFree(free);
			printAlloc(alloc);
			printMemory(memory);
			//printPMT(pmt);
		}
		else
		{
			int pid_to_remove = atoi(tokens[0].c_str());
			map<int,vector<int> > temp = pmt[pid_to_remove];
			pages -= pagesArray[pid_to_remove];
			//vector<int> t(50);
			for(int i=0;i<100000;i++)
			{
				if(matrix[i][1] == pid_to_remove)
				{
					matrix[i][0] = -1;
					matrix[i][1] = -1;
					matrix[i][2] = -1;
				}
				blockArray[i] = 0;
			}
			int ig = 0;
			for(map<int,vector<int> >::iterator it = temp.begin();it!=temp.end();it++)
			{
			//	t[ig++] = it->second[0];
				free[it->second[0]] = 1;
				alloc[it->second[0]] = 0;
			}
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<m;j++)
				{
					if(memory[i][j] == pid_to_remove)
					{
						memory[i][j] = 0;
						int block = (i/(int)sqrt(n))*(int)sqrt(n) + j/(int)sqrt(n);
						for(int i=0;i<n;i++)
						{
							if(bl_pages[i].second == block)
							{
								bl_pages[i].first--;
							}
						}
					}
				}
			}
			pmt.erase(pid_to_remove);
			printFree(free);
			printAlloc(alloc);
			printMemory(memory);
			printPMT(pmt);
		}
		int k = 0;
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				int block_id = (i/(int)sqrt(n))*(int)sqrt(n) + j/(int)sqrt(n);
				//printf("%d\n",block_id );
				if(memory[i][j]!=0)
				{
					int pid = memory[i][j];
					int mem_page = (i*n) + j;		
					matrix[k][0] = block_id;
					matrix[k][1] = pid;
					matrix[k][2] = mem_page;
					k++;
				}
			}
		}

		int block_acquired = 0;
		for(int i=0;i<100000;i++)
		{
			if(matrix[i][0] >= 0)
			{
				//cout<<"B_Id_Acquired : "<< matrix[i][0] << endl;
				blockArray[matrix[i][0]] = 1;
			}
		}
		//printf("n : %d\n",n);
		for(int i=0;i<n;i++)
		{
			if(blockArray[i] == 1)
			{
				block_acquired++;
				blockArray[i] = 0;
				//printf("%d\n",blockArray[i]);
			}
		}
		printf("Block Acquired: %d\n",block_acquired);
		finalmap.clear();
		int anish = k;
		//printMatrix(matrix);
		for (int k = 0; k < n; ++k)
		{
			for (int i = 0; i < anish; ++i)
			{
				if(matrix[i][0] == -1)
				{
					continue;
				}
				if(matrix[i][0] == k)
				{
					//cout<<"Block Occupied Id : "<<k<<" "<<matrix[i][1]<<" "<<matrix[i][2]<<endl;
					cur.push_back(make_pair(matrix[i][1],matrix[i][2]));
				}
			}
			finalmap.insert(pair<int,vector<pair<int,int> > >(k,cur));
			cur.clear();
		}


		map<int,vector<pair<int,int> > >::iterator it = finalmap.begin();
		for (it = finalmap.begin(); it != finalmap.end(); ++it)
		{
			vector<pair<int,int> > temp = it->second;
			for(vector<pair<int,int> >::iterator itx = temp.begin();itx!=temp.end();itx++)
			{
				//printf("blockid - %d -> ",it->first);
				//printf("pid - %d memory_page_number - %d\n",itx->first,itx->second);
			}	
		}

		int block_required = 0;
		if(pages % n == 0)
		{
			block_required = pages/n;
		}
		else
		{
			block_required = (pages/n)+1;
		}
		printf("block required : %d\n",block_required);
		//printf("block acquired : %d\n",block_acquired);
		
		/*if(block_acquired > block_required + 0.2*block_required)
		{
			printf("System using too much power, page migration algorithm is running\n");
			while(block_acquired > block_required)
			{
				int i = 0;
				int j = n;
				if(bl_pages[j].first >= n)
				{
					j--;
					continue;
				}
				if(bl_pages[i].first == 0)
				{
					i++;
					continue;
				}
				if(bl_pages[i].first + bl_pages[j].first <= n)
				{
					int bl_id = bl_pages[i].second;
					int id_to_put = bl_pages[j].second;
					bl_pages[i].first = 0;
					bl_pages[i].second = bl_pages[i].first + bl_pages[j].first;
					i++;
					if(bl_pages[i].second == n)
					{
						j--;
					}
					vector<pair<int,int> > temp = finalmap[bl_id];
					vector<pair<int,int> > temp2 = finalmap[id_to_put];
					finalmap.erase(bl_id);
					temp.insert(temp.end(),temp2.begin(),temp2.end());
					finalmap.erase(id_to_put);
					finalmap.insert(pair<int,vector<pair<int,int> > >(id_to_put,temp));
				}
			}
		}*/
		//printFinalMap(finalmap);
		//printf("hello\n");


		//Page migration Algorithm main loop.
		int a = 0;
		int b = 0;
		//cout<<"see"<<(int)sqrt(n)<<endl;
		for(int i=0; i<n; i++)
		{
			for(int j=0; j<n; j++)	
			{
				if(memory[i][j] != 0)
				{
					memory1[a][b++] = memory[i][j];
					if(b == n && ((a % (int)sqrt(n)) == ((int)sqrt(n) - 1)))
					{
						//cout<<"see here"<<a<<" "<<b<<endl;
						a++;
						b = 0;
					}
					else
					{
						if(b % (int)sqrt(n) == 0)
						{
							a++;
							b = b - (int)sqrt(n);
							if(a != 0 && a % (int)sqrt(n) == 0)
							{
								a = a - (int)sqrt(n);  
								b = b + (int)sqrt(n);
							}
						}
					}	
				}
			}
		}
		printMemory(memory1);
		
		for(int i = 0;i<n;i++)
		{
			for(int j = 0;j<m;j++)
			{
				memory[i][j] = memory1[i][j];
				int mem_page = (i*n) + j;
				if(memory[i][j] != 0)
				{
					free[mem_page] = 0;
					alloc[mem_page] = 1;
				}
				else
				{
					free[mem_page] = 1;
					alloc[mem_page] = 0;
				}
				memory1[i][j] = 0;
			}
		}
			//printf("hello\n");
				/*pmt.clear()
				for(int i = 0; i<n; i++)
				{
					for(int j = 0; j<n; j++)
					{
						if(memory[i][j] == pid)
						{
						int mem_page = (i*n) + j;		//generating the block number using x and y co-ordinate of the memory,
						pv[0] = mem_page;
						int block = (i/(int)sqrt(n))*(int)sqrt(n) + j/(int)sqrt(n);
						pv[1] = block;
							for(int i =0;i<n;i++)
							{
								if(bl_pages[i].second == block)
								{
									bl_pages[i].first++;
								}
							}
						
						temp.insert(pair<int,vector<int> >(ix,pv));
						
						}

						}	
				
				}
					
					pmt.insert(pair<int,map<int,vector<int> > >(p_id,temp));	
						printPMT(pmt);
				*/	

							
		
			

		float memory_used = ((float)block_acquired/n)*100.00;
		float memory_used_pma = ((float)block_required/n)*100.00;
		myfile1 << count1++ << " "<< memory_used << endl;
		myfile2 << count2++ << " "<< memory_used_pma << endl;
		p--;
	}
	/*ifstream ifs("example.txt");      
	string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	std::string str1;
	str1.erase(std::remove(str1.begin(), str1.end(), '\n'), str1.end());*/
	myfile1.close();
	myfile2.close();
	return 0;
}

void printMatrix(int matrix[][3])
{
	printf("\n\n");
	printf("\nBlock_id, pid, memory_page_number\n");
	for(int i=0;i<10000;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(matrix[i][0] != -1)
			{
				printf("%d ",matrix[i][j]);
			}
		}
		if (matrix[i][0]!=-1)
		{
			printf("\n");
		}
	}
	printf("\n\n");
}

void printbl_pages(pair<int,int> bl_pages[])
{
	sort(bl_pages,bl_pages+n);
	printf("\n\nblock and number of pages table\n");
	for(int i=0;i<n;i++)
	{
		printf("%d %d\n",bl_pages[i].second,bl_pages[i].first);
	}
	printf("\n\n");
}

void printFree(int free[])
{
        printf("\nFree list:\n");
        for(int i=0;i<n*m;i++)
        {
                printf("%d ",free[i]);
        }
        printf("\n");
}

void printAlloc(int alloc[])
{
	printf("\nAllocated list:\n");
	for(int i=0;i<n*m;i++)
        {
        	printf("%d ",alloc[i]);
        }
        printf("\n");
}

void printMemory(int memory[][n])
{
	printf("\nMemory\n");
	for(int i=0;i<n;i++)    
        {
        	for(int j=0;j<m;j++)
                {
                	printf("%d ",memory[i][j]);
                }
         	printf("\n");
       	}
}

void printPMT(map<int,map<int,vector<int> > > pmt)
{
	printf("\npage map table\n");
	for(map<int,map<int,vector<int> > >::iterator it = pmt.begin();it!=pmt.end();it++)
    {
            //printf("%d -> ",it->first);
            map<int,vector<int> > temp1 = pmt[it->first];
            for(map<int,vector<int> >::iterator ic = temp1.begin();ic!=temp1.end();ic++)
            {
                    printf("%d -> ",it->first);
                    printf("%d ",ic->first);
                    vector<int> an = ic->second;
                    printf("%d %d\n",an[0],an[1]);
            }
    }
}
