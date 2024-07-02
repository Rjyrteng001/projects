__kernel void fillArray(int a,   //index 0 inputMultiplier
						__global int *b) {     //index 1 in/output array
	
	int i = get_global_id(0); //0 - 512 work items
	int arry[512];
	arry[i] = i;  
	b[i] = (arry[i] * a) + 1;

}
