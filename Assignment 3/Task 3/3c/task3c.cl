__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
      CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST; 

// 1x7 / 7x1

__kernel void darken(read_only image2d_t src_image,
					write_only image2d_t dst_image,
                    float luminance) {
   /* Get pixel coordinate */
   int2 coordinates = (int2)(get_global_id(0), get_global_id(1));

   /* Read pixel value */
   float4 pixel = read_imagef(src_image, sampler, coordinates);

   /* if it is above our threshold value darken it */
   if((pixel.x + pixel.y + pixel.z) / 3 <= luminance){
        pixel.xyz = 0;
   }

   /* Write new pixel value to output */
	write_imagef(dst_image, coordinates, pixel);
}

__kernel void combine(read_only image2d_t src_image,
						   read_only image2d_t src_image_two,
						   write_only image2d_t dst_image) {
	/* Get work-item’s row and column position */
	int column = get_global_id(0); 
	int row = get_global_id(1);

	/* Accumulated pixel value */
	float4 sum = (float4)(0.0);

	float4 pixel;
	int2 coordinates = (int2)(get_global_id(0), get_global_id(1));
   
   	/* read in the RGB value of each pixel in first image and add to the float4 sum pixel value */ 
   	pixel = read_imagef(src_image, sampler, coordinates);
	sum.x = pixel.x;
	sum.y = pixel.y;
	sum.z = pixel.z;
 
    /* read in the RGB value of each pixel in the 2nd image to float4 pixel and 
    combine them from the previous float4 pixel value in sum. */
	pixel = read_imagef(src_image_two, sampler, coordinates);
	
	pixel.x += sum.x;
	pixel.y += sum.y;
	pixel.z += sum.z;

	if((pixel.x + pixel.y + pixel.z) / 3 > 1.0){
		pixel.xyz = 1;
	}

	/* Write new pixel value to output */
	write_imagef(dst_image, coordinates, pixel);
}