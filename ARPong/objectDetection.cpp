#include "ARPong.h"

void calculate_median(int *buffer, int &paddleX, int &paddleY)
{
	//given a buffer with a skin detection filter applied to it,
	// search for the x and y coordinates that approximately bound
	// a hand - where there is skin detected on  X% of a horizontal
	// line and y% of a vertical line. 
	//--dependent on the hand being the primary thing in the picture. 
	int count, xmin, xmax, ymin, ymax;
	int y_threshold = WIDTH/5;
	for(int j=0;j<HEIGHT;j++)
	{
	   count =0;
		for(int i=0; i<WIDTH;i++)
		{
			if(buffer[WIDTH*j+i]!=0)
				count++;
		}
		if (count > y_threshold)
		{
			ymin = j;
			break;
		}
	}
	for(int j=HEIGHT-1;j>=0;j--)
	{
	   count =0;
	   for(int i=0; i<WIDTH;i++)
	   {
			if (buffer[WIDTH*j+i]!=0)
				count++;
	   }
	   if (count > y_threshold)
	   {
		  ymax = j;
		  break;
	   }
	}
	paddleY = (ymin + ymax) / 2;
	int x_threshold = (ymax - ymin)/2;
	for(int i=0; i<WIDTH;i++)
	{
	   count =0;
	   for(int j=ymin;j<ymax;j++)
	   {
		if (buffer[WIDTH*j+i]!=0)
			count++;
	   }
	   if (count > x_threshold)
	   {
			xmin = i;
			break;
	   }
	}
	for(int i=WIDTH-1; i>=0;i--)
	{
	   count=0;
	   for(int j=ymin;j<ymax;j++)
	   {
		if(buffer[WIDTH*j+i]!=0)
			count++;
	   }
	   if(count > y_threshold)
	   {
			xmax = i;
			break;
	   }
	}
	paddleX = (xmin + xmax) / 2;
}