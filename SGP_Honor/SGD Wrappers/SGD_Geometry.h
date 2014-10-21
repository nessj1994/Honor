/***********************************************************************\
|																		|
|	File:			SGD_Geometry.h  									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store rectangle components						|
|					left, top, right, bottom							|
|																		|
|					To store point components							|
|					x, y												|
|																		|
|					To store size components							|
|					width, height										|
|																		|
|					To store vector components							|
|					x, y												|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_GEOMETRY_H
#define SGD_GEOMETRY_H


namespace SGD
{
	// Forward declarations
	class Rectangle;
	class Point;
	class Size;
	class Vector;

	extern const float PI;

	
	//*****************************************************************//
	// Rectangle
	//	- axis-aligned rectangle in 2D space with left, top, right, and bottom sides
	//	- the right and bottom are exclusive (collision and rendering are < those sides)
	class Rectangle 
	{
	public:
		float	left;		// left x position (inclusive)
		float	top;		// top y position (inclusive)
		float	right;		// right x position (exclusive)
		float	bottom;		// bottom y position (exclusive)


#pragma region RECTANGLE_METHODS
		
		Rectangle( void );													// Default constructor (empty)
		Rectangle( float L, float T, float R, float B );					// Overloaded constructor
		Rectangle( const Point& topleft, const Point& bottomright );		// Overloaded Point-Point constructor
		Rectangle( const Point& topleft, const Size& size );				// Overloaded Point-Size constructor
		

		Point		GetTopLeft		( void )	const;
		Point		GetTopRight		( void )	const;
		Point		GetBottomLeft	( void )	const;
		Point		GetBottomRight	( void )	const;

		void		SetTopLeft		( const Point& topleft		);
		void		SetTopRight		( const Point& topright		);
		void		SetBottomLeft	( const Point& bottomleft	);
		void		SetBottomRight	( const Point& bottomright	);


		Point		ComputeCenter		( void )	const;
		Size		ComputeSize			( void )	const;
		float		ComputeWidth		( void )	const;
		float		ComputeHeight		( void )	const;

		bool		IsEmpty				( void )	const;

		bool		IsIntersecting		( const Rectangle& other )	const;
		Rectangle	ComputeIntersection	( const Rectangle& other )	const;
		Rectangle	ComputeUnion		( const Rectangle& other )	const;


		void		Normalize			( void );							// corrects positions so topleft < bottomright

		void		MoveTo				( float left, float top	);			// keeps size
		void		MoveTo				( const Point& topleft	);

		void		Offset				( float deltaX, float deltaY	);	// keeps size
		void		Offset				( const Vector& offset			);

		void		Resize				( float width, float height );		// keeps top-left position
		void		Resize				( const Size& size			);

		void		Inflate				( float width, float height );		// expands all 4 sides
		void		Inflate				( const Size& size			);

		
		bool		operator==			( const Rectangle& other )	const;	// comparison
		bool		operator!=			( const Rectangle& other )	const;

		Rectangle	operator+			( const Vector& offset	)	const;	// keeps size
		Rectangle	operator-			( const Vector& offset	)	const;
		Rectangle&	operator+=			( const Vector& offset	);
		Rectangle&	operator-=			( const Vector& offset	);

		Rectangle	operator+			( const Size&	size	)	const;	// keeps top-left position
		Rectangle	operator-			( const Size&	size	)	const;
		Rectangle&	operator+=			( const Size&	size	);
		Rectangle&	operator-=			( const Size&	size	);

#pragma endregion

	};	// class Rectangle

	
	//*****************************************************************//
	// Point
	//	- (x,y) position in 2D space
	class Point 
	{
	public:
		float	x;
		float	y;


#pragma region POINT_METHODS

		Point( void );														// Default constructor (0, 0)
		Point( float X, float Y );											// Overloaded constructor

		bool		IsWithinRectangle	( const Rectangle& rectangle ) const;

		Vector		ComputeDifference	( const Point& other )		const;	// vector to other point

		void		Offset				( float deltaX, float deltaY	);
		void		Offset				( const Vector& offset			);


		bool		operator==			( const Point& other )		const;	// comparison
		bool		operator!=			( const Point& other )		const;

		Point		operator+			( const Vector& offset )	const;	// addition
		Point		operator-			( const Vector& offset )	const;	// subtraction
		Point&		operator+=			( const Vector& offset );
		Point&		operator-=			( const Vector& offset );

		Point		operator-			( void )					const;	// negation

		Vector		operator-			( const Point& other )		const;	// difference between points

#pragma endregion

	};	// class Point

	
	//*****************************************************************//
	// Size
	//	- width & height in 2D space
	class Size 
	{
	public:
		float	width;
		float	height;


#pragma region SIZE_METHODS

		Size( void );														// Default constructor [0, 0]
		Size( float W, float H );											// Overloaded constructor

		float		ComputeArea			( void )	const;


		operator Vector					( void )	const;					// typecast to Vector
		

		bool		operator==			( const Size& other )		const;	// comparison
		bool		operator!=			( const Size& other )		const;

		Size		operator+			( const Size& offset )		const;	// addition
		Size		operator-			( const Size& offset )		const;	// subtraction
		Size&		operator+=			( const Size& offset );
		Size&		operator-=			( const Size& offset );

		Size		operator*			( float scale )				const;	// scalar multiplication
		Size		operator/			( float scale )				const;	// scalar division
		Size&		operator*=			( float scale );
		Size&		operator/=			( float scale );

		Size		operator-			( void )					const;	// negation

#pragma endregion

	};	// class Size

	
	//*****************************************************************//
	// Vector
	//	- <x,y> offset in 2D space
	class Vector 
	{
	public:
		float	x;
		float	y;


#pragma region VECTOR_METHODS
		
		Vector( void );														// Default constructor <0, 0>
		Vector( float X, float Y );											// Overloaded constructor

		float		ComputeLength		( void )	const;

		void		Normalize			( void );
		Vector		ComputeNormalized	( void )	const;

		void		Rotate				( float radians );
		Vector		ComputeRotated		( float radians )	const;

		float		ComputeDotProduct	( const Vector& other )		const;
		float		ComputeAngle		( const Vector& other )		const;
		float		ComputeSteering		( const Vector& other )		const;


		bool		operator==			( const Vector& other )		const;	// comparison
		bool		operator!=			( const Vector& other )		const;

		Vector		operator+			( const Vector& offset )	const;	// addition
		Vector		operator-			( const Vector& offset )	const;	// subtraction
		Vector&		operator+=			( const Vector& offset );
		Vector&		operator-=			( const Vector& offset );

		Vector		operator*			( float scale )				const;	// scalar multiplication
		Vector		operator/			( float scale )				const;	// scalar division
		Vector&		operator*=			( float scale );
		Vector&		operator/=			( float scale );
		
		Vector		operator-			( void )					const;	// negation

#pragma endregion

	};	// class Vector

}	// namespace SGD

#endif	//SGD_GEOMETRY_H
