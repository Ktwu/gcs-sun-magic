//Author : Stefan Woskowiak
//www.rixcode.com

#pragma once

#include "stdafx.h"

// SFML vector math
namespace sfm
{
	const float PI = 3.14159f;
	static float RADTODEG(const float Radians) { return Radians * 180.0f / PI; }
	static float DEGTORAD(const float Degrees) { return Degrees * PI / 180.0f; }

	//Zero out a vector
	template <class T>
	void Zero(sf::Vector3<T> &vec) { vec.x = vec.y = vec.z = 0; }

	template <class T>
	void Zero(sf::Vector2<T> &vec) { vec.x = vec.y = 0; }

	//Negate a vectors values
	template <class T>
	void Negate(sf::Vector3<T> &vec) { vec.x = -vec.x; vec.y = -vec.y; vec.z = -vec.z; }

	template <class T>
	void Negate(sf::Vector2<T> &vec) { vec.x = -vec.x; vec.y = -vec.y; }

	//Get the length of a vector
	template <class T>
	float Length(const sf::Vector3<T> &vec){	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);	}

	template <class T>
	float Length(const sf::Vector2<T> &vec){	return sqrt(vec.x * vec.x + vec.y * vec.y);	}

	//Get the length of a vector
	template <class T>
	float LengthSquared(const sf::Vector3<T> &vec){	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;	}

	template <class T>
	float LengthSquared(const sf::Vector2<T> &vec){	return vec.x * vec.x + vec.y * vec.y;	}

	//Normalize the length of a vector
	template <class T>
	void Normalize(sf::Vector3<T> &vec){ float len = Length(vec);	vec/=(len); }

	template <class T>
	void Normalize(sf::Vector2<T> &vec){ float len = Length(vec);	vec/=(len); }

	//Rotate a vector ( available for 2D only ) 
	template <class T>
	void Rotate(sf::Vector2<T> &vec, float angleInDegrees){
		float temp = DEGTORAD(angleInDegrees);
		float sin1 = sin(temp);
		float cos1 = cos(temp);
		tempx = (vec.x * cos1) - (vec.y * sin1);
		vec.y = (vec.y * cos1) + (vec.x * sin1);
		vec.x = tempx;
	}
	
	//Calculate angle between two vectors and returns it in Radians
	template <class T>
	float AngleBetweenR(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		float len1 = Length(v1);
		float len2 = Length(v2);
		if (!len1 || !len2)
			return 0;
		float angle = acosf(Dot(v1, v2) / (len1 * len2));
		if ( std::numeric_limits(angle))
			return 0;
		return angle;
	}

	template <class T>
	float AngleBetweenR(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return acos( Dot(v1, v2) );
	}
	//Calculate angle between two vectors and returns it in degrees. 
	template <class T>
	float AngleBetweenD(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		return RADTODEG(AngleBetweenR(v1, v2));
	}

	template <class T>
	float AngleBetweenD(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return RADTODEG(AngleBetweenR(v1, v2));
	}

	//Calculate dot product of two 3D vectors
	template <class T>
	float Dot(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <class T>
	float Dot(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return v1.x * v2.x + v1.y * v2.y;
	}

	//Calculate the cross product of two 3D vectors
	template <class T>
	sf::Vector3<T> Cross(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2)
	{
		sf::Vector3<T> vResult;
		vResult.x = v1.y * v2.z - v1.z * v2.y;
		vResult.y = v1.z * v2.x - v1.x * v2.z;
		vResult.z = v1.x * v2.y - v1.y * v2.x;
		return vResult;
	}

	template <class T>
	float Cross(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2)
	{
		return((v1.x * v2.y) - ( v1.y * v2.x ));
	}

	//Convert a 2D vector to a 3D vector ( nothing fancy just for a time saver )
	template <class T>
	sf::Vector3<T> Convert2Dto3D(const sf::Vector2<T> &vec) { return sf::Vector3<T>(vec.x, vec.y, 0); }

	inline float squaredDistance(sf::Vector2f v1, sf::Vector2f v2) {
		return pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2);
	}
	
	// Create a rectangle from p1 to p2
	inline void SetToLine(sf::RectangleShape &rect, sf::Vector2f p1, sf::Vector2f p2, float thickness) {
		sf::Vector2f diff = p2 - p1;
		sf::Vector2f size(sfm::Length(diff), thickness);
		rect.setSize(size);
		rect.setPosition(0.5f * (p1 + p2));
		rect.setOrigin(0.5f * size);
		rect.setRotation(sfm::RADTODEG(atan2(diff.y, diff.x)));
	}
}