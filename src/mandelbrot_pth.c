/** @file 	mandelbrot_pth.c
 *	@brief	Pthread parallel program to compute and plot mandelbrot set
 *	
 *	C implementation of a pthread parallel program to compute and plot some
 *  subset of a mandelbrot set
 *
 *	Usage:
 *    ./mandelbrot_pth c_x_min c_x_max c_y_min c_y_max image_size ... 
 *		- c_x_min: Lowest x boundary for the figure to be computed
 *		- c_x_max: Highest x boundary for the figure to be computed
 *		- c_y_mix: Lowest y boundary for the figure to be computed
 *		- c_y_max: Highest y boundary for the figure to be computed
 *		- image_size: The resolution of the resulting image
 *		- color_scheme: An integer in [0,5] representing the color scheme
 *    It also accept a flag a -nt # including the number of threads
 *
 *  Usage examples:
 *      Full Picture:         ./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500 0
 *      Seahorse Valley:      ./mandelbrot_pth -0.8 -0.7 0.05 0.15 11500 2
 *      Elephant Valley:      ./mandelbrot_pth 0.175 0.375 -0.1 0.1 11500 5
 *      Triple Spiral Valley: ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500
 *
 *	Notes:
 *      The program uses default color scheme 0 if not provided.
 *      Only two ways for using the -nt (number of threads) flag are accepted:
 *			-Right after the image_size, if no chosen color scheme (default)
 *			-Right after the color_scheme, if provided
 *	Examples of valid calls:
 *		- 4 Threads with color scheme assigned:
 *      	./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500 4 -nt 4
 *		- 8 Threads using default color scheme:
 *          ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500 -nt 8
 *
 *  About the color scheme:
 *  	The color scheme executes a weighing based on the magnitude of the
 *		number of iterations and its value in hexadecimal and assign this
 *		value to the corresponding color (dependent of the color scheme)
 *
 *	@author		Candy Tenorio Gonzales (cvtenoriog@gmail.com)
 *	@author		Decio Lauro Soares (deciolauro@gmail.com)
 *	@author		Fatemeh Mosaiyebzadeh (mahshid.msy179@gmail.com)
 *	@date		15 Apr 2017
 *	@bug		No known bugs
 *	@warning	The number of threads flag should come after all parameters
 *  @warning	One must edit the PLOT_IMAGE if it want the plot
 * 	@copyright	GNU Public License v3
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // parse -np flag
#include <pthread.h>

#define UC unsigned char

/* GLOBAL VARIABLES - BEGIN */
double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;
double pixel_width;
double pixel_height;

int iteration_max = 0xFFF;  // Scale of each color from 0 to F
int GLOBAL_SCHEME = 0; // 0-BGR, 1-GRB, 2-RGB, 3-BRG, 4-GBR, 5-RBG

int image_size;
UC **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;
int NUM_THREADS = 4; // By default the program uses 4 threads
int PLOT_IMAGE = 0;  // By default do not plot images

/* GLOBAL VARIABLES - END */

/**
 * @brief Auxiliary function to allocate the image buffer
 *
 * Auxiliary function that allocate the image buffer based on the value of
 * \a image_size passed as parameter during the call
 */
void allocate_image_buffer()
{
    int rgb_size = 3;
    image_buffer = (UC **) malloc(sizeof(UC *) * image_buffer_size);

    for(int i = 0; i < image_buffer_size; i++)
	{
        image_buffer[i] = (UC *) malloc(sizeof(UC) * rgb_size);
    };
}


/**
 * @brief Function responsible for initialize all global variables
 *
 * This function is responsible for parsing the parameters during the call and
 * initialize/change the value of some global variables
 *
 * @param argc An integer with the argc value
 * @param argv A char** with the argv parameters
 */
void init(int argc, char *argv[])
{
    if(argc < 6)
	{
        printf("usage:\n./mandelbrot_pth c_x_min c_x_max c_y_min c_y_max image_size color_scheme(optional default=0) ...\n");
        printf("examples with image_size = 11500:\n");
        printf("    Full Picture:         ./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500 0\n");
        printf("    Seahorse Valley:      ./mandelbrot_pth -0.8 -0.7 0.05 0.15 11500 3\n");
        printf("    Elephant Valley:      ./mandelbrot_pth 0.175 0.375 -0.1 0.1 11500 5 -nt 12\n");
        printf("    Triple Spiral Valley: ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500 -nt 8\n");
        exit(0);
    }
    else
	{
        sscanf(argv[1], "%lf", &c_x_min);
        sscanf(argv[2], "%lf", &c_x_max);
        sscanf(argv[3], "%lf", &c_y_min);
        sscanf(argv[4], "%lf", &c_y_max);
        sscanf(argv[5], "%d", &image_size);
		if(argc >= 7)
		{
			if(strcmp(argv[6], "-nt")==0)
			{
				sscanf(argv[7], "%d", &NUM_THREADS);
			}
			else
			{
				sscanf(argv[6], "%d", &GLOBAL_SCHEME);
				if(argc > 7 && strcmp(argv[7], "-nt")==0)
				{
					sscanf(argv[8], "%d", &NUM_THREADS);
				}
			}
		}

        i_x_max           = image_size;
        i_y_max           = image_size;
        image_buffer_size = image_size * image_size;

        pixel_width       = (c_x_max - c_x_min) / i_x_max;
        pixel_height      = (c_y_max - c_y_min) / i_y_max;
    };
}


/**
 * @brief The function responsible for update the pixel color after calculation
 *
 * This function is responsible to update the pixel color based on the
 * pixel coordinates, the iteration value during the divergence and the color
 * scheme
 *
 * @param iteration An int with the number of iterations until the divergence
 * @param x An int value with the x coordinate for the pixel
 * @param y An int value with the y coordinate for the pixel
 * @param color_sch An in value with the color scheme defined above
 */
void update_rgb_buffer(int iteration, int x, int y, int color_sch)
{
	int first_color  = (iteration & 0xF00) >> 8;
	int second_color = (iteration & 0x0F0) >> 4;
	int third_color  = (iteration & 0x00F);

	if(iteration >= iteration_max)
	{
		first_color = 0;
		second_color = 0;
		third_color = 0;
	}
	else
	{
		//scaling to 255
		first_color  = 17*first_color;
		second_color = 17*second_color;
		third_color  = 17*third_color;
	}
	//check the color scheme [0]->B, [1]->G, [2]->R
	if(color_sch == 0)
	{
		image_buffer[(i_y_max * y) + x][0] = third_color;
		image_buffer[(i_y_max * y) + x][1] = second_color;
		image_buffer[(i_y_max * y) + x][2] = first_color;
	}
	else if(color_sch == 1)
	{
		image_buffer[(i_y_max * y) + x][0] = second_color;
		image_buffer[(i_y_max * y) + x][1] = first_color;
		image_buffer[(i_y_max * y) + x][2] = third_color;
	}
	else if(color_sch == 2)
	{
		image_buffer[(i_y_max * y) + x][0] = first_color;
		image_buffer[(i_y_max * y) + x][1] = third_color;
		image_buffer[(i_y_max * y) + x][2] = second_color;
	}
	else if(color_sch == 3)
	{
		image_buffer[(i_y_max * y) + x][0] = first_color;
		image_buffer[(i_y_max * y) + x][1] = second_color;
		image_buffer[(i_y_max * y) + x][2] = third_color;
	}
	else if(color_sch == 4)
	{
		image_buffer[(i_y_max * y) + x][0] = third_color;
		image_buffer[(i_y_max * y) + x][1] = first_color;
		image_buffer[(i_y_max * y) + x][2] = second_color;
	}
	else
	{
		image_buffer[(i_y_max * y) + x][0] = second_color;
		image_buffer[(i_y_max * y) + x][1] = third_color;
		image_buffer[(i_y_max * y) + x][2] = first_color;
	}
}


/**
 * @brief Write the image to file using a .ppm type.
 */
void write_to_file()
{
    FILE * file;
    char * filename               = "output.ppm";
    char * comment                = "# ";

    int max_color_component_value = 255;
	int i = 0;

    file = fopen(filename,"wb");

    fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment,
            i_x_max, i_y_max, max_color_component_value);

    for(i = 0; i < image_buffer_size; i++)
	{
        fwrite(image_buffer[i], 1 , 3, file);
    };

    fclose(file);
}


/**
 * @brief Pthread function to parallelize the calculations
 *
 * This function is responsible for parallelize part of the structure
 * implemented in the sequential implementation
 *
 * @param nt A void pointer to be cast to a int that stores the thread id
 */
void *iterate_y(void *nt)
{
	int st = *((int *) nt);
	double z_x;
    double z_y;
    double z_x_squared;
    double z_y_squared;
    double escape_radius_squared = 4;

    int iteration = 0;
    int i_x;
    int i_y;

    double c_x;
    double c_y;

	for(i_y = st - 1; i_y < i_y_max; i_y += NUM_THREADS)
	{
		c_y = c_y_min + i_y * pixel_height;

        if(fabs(c_y) < pixel_height / 2)
		{
            c_y = 0.0;
        };

        for(i_x = 0; i_x < i_x_max; i_x++)
		{
            c_x         = c_x_min + i_x * pixel_width;

            z_x         = 0.0;
            z_y         = 0.0;

            z_x_squared = 0.0;
            z_y_squared = 0.0;

            for(iteration = 0;
                iteration < iteration_max && \
                ((z_x_squared + z_y_squared) < escape_radius_squared);
                iteration++)
			{
                z_y         = 2 * z_x * z_y + c_y;
                z_x         = z_x_squared - z_y_squared + c_x;
                z_x_squared = z_x * z_x;
                z_y_squared = z_y * z_y;
            };

            update_rgb_buffer(iteration, i_x, i_y, GLOBAL_SCHEME);
        };
    };
	free(nt);
	return NULL;
}


int main(int argc, char *argv[])
{
	int i;
    init(argc, argv);
	pthread_t thread[NUM_THREADS+1]; // Because of the need to start at 1

	//if(PLOT_IMAGE)
	{
    	allocate_image_buffer();
	}
	
	for(i=1; i<=NUM_THREADS; i++)
	{
		int *aux = malloc(sizeof(*aux));
		if(aux == NULL)
		{
			fprintf(stderr, "Unable to allocate memory for thread argument.\n");
			exit(EXIT_FAILURE);
		}

		*aux = i;
		if(pthread_create(&thread[i], 0, iterate_y, aux) != 0)
		{
			fprintf(stderr, "Unable to create the threads\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Main thread wait
	for (i=1; i <= NUM_THREADS; i++)
	{
    	pthread_join(thread[i], NULL);
	}
	
	if(PLOT_IMAGE)
	{
    	write_to_file();
	}
    
	return 0;
}
