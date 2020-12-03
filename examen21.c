#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define v 1 /* verbose flag, output if 1,
no output if 0 */
int main ( int argc, char *argv[] ){
	int myid,j;
	double *data,tosum[50],sums[2];
	
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	if(myid==0) /* manager allocates
	and initializes the data */
	{
		data = (double*)calloc(100,sizeof(double));
		for (j=0; j<100; j++) data[j] = 0.1;
			if(v>0){
				printf("The data to sum : ");
				for (j=0; j<100; j++) printf(" %f",data[j]);
				}
			}
	printf("\n");
	
	MPI_Scatter(data,50,MPI_DOUBLE,tosum,50,MPI_DOUBLE,0,
	MPI_COMM_WORLD);	
	if(v>0) /* after the scatter,
	every node has 25 numbers to sum */
	{
		printf("Node %d has numbers to sum :",myid);
		for(j=0; j<50; j++) printf(" %f", tosum[j]);
		printf("\n");
	}
	sums[myid] = 0;
	for(j=0; j<50; j++) sums[myid] += tosum[j];
	if(v>0) printf("Node %d computes the sum %f\n", myid,sums[myid]);
	
	MPI_Gather(&sums[myid],1,MPI_DOUBLE,sums,1,MPI_DOUBLE,0,
	MPI_COMM_WORLD);
	if(myid==0) /* after the gather,
	sums contains the four sums */
	{
		printf("The four sums : ");
		printf("%f",sums[0]);
		for(j=1; j<2; j++) printf(" + %f", sums[j]);
		for(j=1; j<2; j++) sums[0] += sums[j];
		printf(" = %f, which should be 5050.\n",
		sums[0]);
	}
	MPI_Finalize();
	return 0;
}