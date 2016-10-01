#pragma once

class Shape
{
public:
	POINT startingPoint;
	POINT finalPoint;

	bool isDrawing;
	int thickness;
	COLORREF penColor;
	COLORREF brushColor;

	Shape::Shape()
	{
		isDrawing = true;
	}

	Shape::~Shape()
	{}

	virtual void  draw(HDC hdc)
	{}

	virtual void Shape::setStartPosition(int x, int y)
	{
		startingPoint.x = x;
		startingPoint.y = y;
	}

    void Shape::setFinalPosition(int x, int y)
	{
		finalPoint.x = x;
		finalPoint.y = y;
	}

	void Shape::pan(int x, int y){
		int tempX;
		int tempY;
		startingPoint.x += x;
		startingPoint.y += y;
		finalPoint.x += x;
		finalPoint.y += y;
	}

};
