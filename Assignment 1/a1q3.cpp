#include "pixmap.h" 


bool fill(PixMap& image, const Pixel& fillColour, int x, int y) {

	Pixel pixel_clr;
	pixel_clr = image.getPixel(x, y);  //get and store color at pixel location
	
	image.setPixel(fillColour, x, y); //update and set color at coords of image 

	
	if (x < image.width() && y < image.height()) { //check if its within the image boundary


		if (x < image.width() - 1 && pixel_clr == image.getPixel(x + 1, y)) { //checking pixel to right of current pixel and if it had original color of current pixel
			fill(image, fillColour, x + 1, y);         //fill image in with color      
		}

		if (x > 0 && pixel_clr == image.getPixel(x - 1, y)) { //checking pixel to left of current pixel and if it had original color of current pixel
			fill(image, fillColour, x - 1, y);
		}

		if (y < image.height() - 1 && pixel_clr == image.getPixel(x, y + 1)) { //checking above current pixel and if it had original color of current pixel
			fill(image, fillColour, x, y + 1);
		}
		if (y > 0 && pixel_clr == image.getPixel(x, y - 1)) { //checking below current pixel and if it had original color of current pixel
			fill(image, fillColour, x, y - 1);
		}
		
	}
	else {
		return false;
	}


}