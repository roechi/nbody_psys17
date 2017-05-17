/**
 * @file hellocl.c
 *
 * This is a simple example in which the string
 *  "Hello, OpenCL!"
 * is copied by a OpenCL kernel. The result of this
 * operation is then displayed by the CPU.
 */

#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

/**
 * Read the content of the file given by the filename.
 *
 * @param filename the name of the file which should be read.
 *
 * @return the contents or NULL on an error. The returned
 *  value must be freed with free() when no longer in use.
 */
static char *read_file_contents(const char *filename)
{
	long size;
	char *ret = NULL;
	char *contents = NULL;
	FILE *fh;

	if (!(fh = fopen(filename,"r")))
		return NULL;

	fseek(fh,0,SEEK_END);
	size = ftell(fh);
	if (size < 1)
		goto out;
	fseek(fh,0,SEEK_SET);

	if (!(contents = malloc(size+1)))
		goto out;
	if ((fread(contents, 1, size, fh) != size))
		goto out;

	contents[size] = 0;

	ret = contents;
	contents = NULL;
out:
	fclose(fh);
	free(contents);
	return ret;
}

/**
 * Our main program. What we are doing here is following.
 *
 * 1) Initialize OpenCL (find platform and device)
 * 2) Create and initialize the context
 *   a) Create the context
 *   b) Create a command queue and attach it to the context
 *   c) Load the kernel and compile it for the context
 *   d) Create the device buffers on which the kernel operates
 *   e) Plug the device buffers to the proper argument positions
 *      of the kernel
 * 3) Prepare and run the calculation
 *   a) Insert a command to copy the data from host buffer to
 *      above-created device buffer
 *   b) Insert a command to invoke the kernel computation. This
 *      operates on the device buffer we plugged into the kernel
 *      and produces something in the second device buffers also
 *      created above.
 *   c) Insert a command to copy the contents of the second device
 *      buffer into a second host buffer
 *   d) Wait for all the action to be completed
 * 4) Print out the contents of the second host buffer
 * 5) Cleanup
 */
int main() 
{
	float a[5] = {1,2,3,4,5};
	float b[5] = {6,7,8,9,10};
	float dest[5] = {-1,-1,-1,-1,-1};

	/* Defines the number of work items, we consider each character as a work item */
	size_t num_bytes = 5*sizeof(float);
	size_t length = 5;

	/* The error variable. We need it everywhere */
	cl_int error;

	/* Phase 1 variables */
	cl_platform_id platform;
	cl_device_id device;
	cl_uint platforms, devices;
	char name[128];

	/* Phase 2 variables */
	char *k_src = NULL; /* kernel source */
	cl_context context = NULL;

	/* Exit value, we assume that we fail */
	int rc = EXIT_FAILURE;

	/*** Here we go ***/

	/**** Phase 1: Find platform and device (will OpenCL work at all?) *****/

	/* Fetch available platform; we only want the first one. */
	error = clGetPlatformIDs(1, &platform, &platforms);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't get platform ids\n");
		goto out;
	}
	printf("Number of available platforms = %d\n",platforms);
	
	/* Get the name of the platform and print it */
	error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
	if (error != CL_SUCCESS) 
	{
		fprintf(stderr,"Couldn't get platform name\n");
		goto out;
	}
	printf("Platform = %s\n",name);

	/* Now get the first device of our platform */
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &devices);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't get device ids\n");
		goto out;
	}

	char deviceName[128];
	error = clGetDeviceInfo(device,CL_DEVICE_NAME,sizeof(deviceName),deviceName,NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't get device name\n");
	}
	printf("Device = %s\n",deviceName);

	/**** Phase 2: Create the context and all of its associates *****/

	/* Create a CL context for the selected device, note that nVidia's OpenCL requires the platform property */
	cl_context_properties properties[]=
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
		0
	};
	context = clCreateContext(properties, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create the context\n");
		goto out;
	}
	
	/* Now create the command queue */
	cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create the command queue\n");
		goto out;                
	}

	/* Now read in the file with the kernel */
    if (!(k_src = read_file_contents("../kernel/core.cl")))
    {
		fprintf(stderr,"Couldn't read the contents of the kernel file!\n");
		goto out;
    }

    /* Array of all sources, we only have one source file */
	const char *srcptr[]={k_src};

	/* Submit the just loaded source code to OpenCL. The resulting
	 * program/kernel is associated with the given context */
	cl_program prog = clCreateProgramWithSource(context, 1, srcptr, NULL, &error);
	if (error != CL_SUCCESS) 
	{
		fprintf(stderr,"Couldn't create the kernel from source\n");
		goto out;
	}

	/* Now compile */
	error = clBuildProgram(prog, 0, NULL, "", NULL, NULL);
	if (error != CL_SUCCESS )
	{
		char build_c[4096];

		fprintf(stderr, "Error on buildProgram\n");
		fprintf(stderr, "\nRequestingInfo\n");
		clGetProgramBuildInfo( prog, device, CL_PROGRAM_BUILD_LOG, sizeof(build_c), build_c, NULL );
		fprintf(stderr, "Build Log for %s_program:\n%s\n", "example", build_c );
		goto out;
	}

	/* Allocate the input memory for the kernel. In the kernel, we only read from it,
	 * hence we declare it as read only. */
	cl_mem cl_amem, cl_bmem, cl_destmem;
	cl_amem = clCreateBuffer(context, CL_MEM_READ_ONLY, num_bytes, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}

	cl_bmem = clCreateBuffer(context, CL_MEM_READ_ONLY, num_bytes, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}

	cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, num_bytes, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}
   

	/* We now get access to a kernel handle */
	cl_kernel k_example = clCreateKernel(prog, "add_vec", &error);
	if (error != CL_SUCCESS){
		fprintf(stderr,"Couldn't create kernel!\n");
		goto out;
	}
	

	
	error =  clSetKernelArg(k_example, 0, sizeof(cl_mem), &cl_destmem);
	printf("aha %p\n",k_example);	
	if (error != CL_SUCCESS){
		fprintf(stderr,"Couldn't set argument for kernel!\n");
		goto out;
	}

	error = clSetKernelArg(k_example, 1, sizeof(cl_mem), &cl_amem);
	if (error != CL_SUCCESS) {
		fprintf(stderr,"Couldn't set argument for kernel!\n");
		goto out;
	}

	error = clSetKernelArg(k_example, 2, sizeof(cl_mem), &cl_bmem);
	if (error != CL_SUCCESS) {
		fprintf(stderr,"Couldn't set argument for kernel!\n");
		goto out;
	}

	error = clSetKernelArg(k_example, 3, sizeof(size_t), &length);
	if (error != CL_SUCCESS){
		fprintf(stderr,"Couldn't set argument for kernel!\n");
		goto out;
	}


	


	/**** Phase 3: Fill and run the command queue *****/

	/* Send input data to OpenCL (async, don't alter the buffer!) */
	//error = clEnqueueWriteBuffer(cq, cl_destmem, CL_FALSE, 0, worksize, dest, 0, NULL, NULL);
	error = clEnqueueWriteBuffer(cq, cl_amem, CL_FALSE, 0, num_bytes, a, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}

	error = clEnqueueWriteBuffer(cq, cl_bmem, CL_FALSE, 0, num_bytes, b, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't transfer source to target mem!\n");
		goto out;
	}

	/* Start the "calculation" */
	error = clEnqueueNDRangeKernel(cq, k_example, 1, NULL, &length, NULL, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't enqueue the kernel!\n");
		goto out;
	}

	/* Read the result back into buf2 */
	error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, num_bytes, dest, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't read buffer back!\n");
		goto out;
	
	}

	/* We have now submitted all commands into the queue. As this was done asynch, we have to
	 * wait for completion of all the commands */
	error = clFinish(cq);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Unable to finish calculation!\n");
		goto out;
	}
    

	for (int i = 0; i < length; ++i)
	{
		printf("%f, ",dest[i]);
	}
    

	rc = EXIT_SUCCESS;
out:
	if (error != CL_SUCCESS)
		fprintf(stderr,"Error number %d\n", error);

	/**** Phase 4: Clean up  *****/
	if (cl_amem) clReleaseMemObject(cl_amem);
	if (cl_bmem) clReleaseMemObject(cl_bmem);
	if (cl_destmem) clReleaseMemObject(cl_destmem);
	if (context) clReleaseContext(context);

	free(k_src);

	return rc;
}
