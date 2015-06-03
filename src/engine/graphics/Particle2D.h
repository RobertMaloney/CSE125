#ifndef PARTICLE_2D_H
#define PARTICLE_2D_H

//#include <math.h>

//#include "Vec2.hpp"
//#include "Colour4f.hpp"

class Particle2D
{
protected:
	glm::vec2  location;  // The current location of the particle
	glm::vec2  speed;     // The current speed of the particle

	glm::vec4 color;       // The colour of the particle

	int size;              // The size of the particle i.e. how big the pointsprite will be drawn

	int framesToLive;      // Number of frames before the particle is destroyed

public:
	// ---------- Static particle parameters -----------

	static const int FRAMES_TO_LIVE = 50;





	static double randRange(double min, double max)
	{
		return min + (((double)rand() / (double)RAND_MAX) * (max - min));
	}

	static float randRange(float min, float max)
	{
		return min + (((float)rand() / (float)RAND_MAX) * (max - min));
	}

	static int randRange(int min, int max)
	{
		//return min + (((int)rand() % (int)RAND_MAX) * (max - min));
		return ((int)rand() % (max - min + 1)) + min;
	}
	void fullyRandomise()
	{
		color.x = randRange(0.0f, 1.0f);
		color.y = randRange(0.0f, 1.0f);
		color.z = randRange(0.0f, 1.0f);
		color.a = randRange(0.0f, 1.0f);
	}
	// ---------- Constructors -------------------------

	// Constructor that takes an initial position as a Vec2
	Particle2D(glm::vec2 initialLocation)
	{
		location = initialLocation; // Set the particle location

		initialise();
	}

	// Constructor that takes an initial position as values (instead of a Vec2) and a deltaTime value to scale the initial speeds by
	Particle2D(int xLocation, int yLocation)
	{
		location.x = xLocation;
		location.y = yLocation;

		speed.x = randRange(-5.0f, 5.0f),
		speed.y = randRange(0.0f, 6.0f);

		initialise();
	}

	// Constructor that takes a position and a speed
	Particle2D(glm::vec2  initialLocation, glm::vec2  initialSpeed)
	{
		location = initialLocation;
		speed = initialSpeed;

		initialise();
	}

	// ---------- Helper Methods and Getters & Setters -------------------------

	// Method to setup other properties of the particle
	void initialise()
	{
		// Set the initial framesToLive count (this counts down to 0 - at 0 the particle is removed)
		framesToLive = FRAMES_TO_LIVE;

		// Set an entirely random colour (including random alpha value)
		 fullyRandomise();
	
		// Randomise the size of the particle
		size = randRange(10,64);
	}

	float getRed()   { return color.x; }
	float getGreen() { return color.y; }
	float getBlue()  { return color.z; }
	float getAlpha() { return color.a; }

	void setColor(const glm::vec4 &theColour)
	{
		color = theColour;
	}

	void interpolateColourWith(const glm::vec4 &destination, float mixFactor)
	{
		color.x = color.x * mixFactor + (destination.x * (1.0f - mixFactor));
		if (color.x > 1.0f)
			color.x = 1.0f;

		color.y = color.y* mixFactor + (destination.y * (1.0f - mixFactor));
		if (color.y > 1.0f)
			color.y = 1.0f;

		color.z = color.z * mixFactor + (destination.z * (1.0f - mixFactor));
		if (color.z > 1.0f)
			color.z = 1.0f;

		color.a = color.a *mixFactor + (destination.a * (1.0f - mixFactor));
		if (color.a  > 1.0f)
			color.a  = 1.0f;
	}

	void setInterpolatedColour(const glm::vec4 &source, const glm::vec4 &destination, float mixFactor)
	{
		color.x = source.x * mixFactor + (destination.x * (1.0f - mixFactor));
		if (color.x > 1.0f)
			color.x = 1.0f;

		color.y = source.y * mixFactor + (destination.y * (1.0f - mixFactor));
		if (color.y > 1.0f)
			color.y = 1.0f;

		color.z = source.z * mixFactor + (destination.z * (1.0f - mixFactor));
		if (color.z > 1.0f)
			color.z = 1.0f;

		color.a = source.a * mixFactor + (destination.a * (1.0f - mixFactor));
		if (color.a > 1.0f)
			color.z = 1.0f;
	}

	glm::vec2 getLocation()  { return location; }
	float getXLocation()       { return location.x; }
	float getYLocation()       { return location.y; }

	int getSize() { return size; }

	int getFramesToLive() { return framesToLive; }


	// ---------- Main Methods To Define Particle Behaviour -------------------------

	void update()
	{
		// Calculate the new Y speed of the particle
		speed.y = speed.y;

		// Update the position of the particle by the speed it's moving at
		location += speed;

		framesToLive--; // Decrease the frames the particle will live for by 1
	}

};

#endif
