#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <utility>
#include <cmath>

typedef std::pair<double, double> DoublePoint;

template <typename T>
class Figure {
public:
    typedef std::pair<T, T> Point;

    bool Square = false, Rectangle = false, Trapezoid = false;
    bool Abstract = false;

    Figure() : centre(DoublePoint(0,0)), area(0)
    {
        for (int i = 0; i < 4; ++i)
            p[i].first = p[i].second = 0;
    }

    DoublePoint centre;
    double area{ 0 };
    Point p[4];
};

bool DoubleEqual(double lhs, double rhs){
    const double EPS = 0.00001;
    if (lhs > rhs)
        return (lhs - rhs) < EPS ? true : false;
    else
        return (rhs - lhs) < EPS ? true : false;
}

bool IsRight(std::pair<double, double> a, std::pair<double, double> b, std::pair<double, double> c){
    std::pair<double, double> vec1 = { b.first - a.first, b.second - a.second };
    std::pair<double, double> vec2 = { c.first - a.first, c.second - a.second };

    double result = vec1.first * vec2.first + vec1.second * vec2.second;
    if (DoubleEqual(result, 0)) return true;
    return false;
}

bool IsRight(std::pair<int, int> a, std::pair<int, int> b, std::pair<int, int> c){
    std::pair<int, int> vec1 = { b.first - a.first, b.second - a.second };
    std::pair<int, int> vec2 = { c.first - a.first, c.second - a.second };

    int result = vec1.first * vec2.first + vec1.second * vec2.second;
    if (result == 0) return true;
    return false;
}

//-----------------------------------------------------------------------------------------------------------
template <class T>
inline double distance(std::pair<T, T> a, std::pair<T, T> b){
    return (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
}


template <class T>
bool IsRectangle(std::pair<T, T> p[4]){
    std::pair<T, T> null(0, 0);
    if (p[0] == null && p[1] == null && p[2] == null && p[3] == null) return false;

    if (
        IsRight(p[2], p[1], p[3]) &&
        IsRight(p[3], p[2], p[0]) &&
        IsRight(p[1], p[0], p[2]) &&
        IsRight(p[0], p[3], p[1])
        ) return true;
    return false;
}
//-----------------------------------------------------------------------------------------------------------
template <class T>
bool IsSquare(std::pair<T, T> p[4]){
    if (
        IsRectangle(p) &&
        DoubleEqual(distance(p[0], p[1]), distance(p[1], p[2])) &&
        DoubleEqual(distance(p[1], p[2]), distance(p[2], p[3])) &&
        DoubleEqual(distance(p[2], p[3]), distance(p[3], p[0])) &&
        DoubleEqual(distance(p[3], p[0]), distance(p[0], p[1]))
        ) return true;
    return false;
}
//-----------------------------------------------------------------------------------------------------------
template <class T>
bool IsTrapezoid(std::pair<T, T> p[4]){
    std::pair<T, T> null(0, 0);
    if (p[0] == null && p[1] == null && p[2] == null && p[3] == null) return false;

    if (
    	!DoubleEqual(distance(p[1], p[2]), distance(p[3], p[0])) &&
    	DoubleEqual(distance(p[0], p[1]), distance(p[2], p[3])) &&
        DoubleEqual(distance(p[0], p[2]), distance(p[1], p[3]))
        ) return true;
    return false;
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
template <class T>
DoublePoint Centre(Figure<T>& fig){
	if(fig.Square || fig.Rectangle || fig.Trapezoid){
		DoublePoint res(0, 0);
		for (int i = 0; i < 4; ++i)
		{
		    res.first += fig.p[i].first;
		    res.second += fig.p[i].second;
		}
		res.first /= 4;
		res.second /= 4;
		return res;
	}
}

//-----------------------------------------------
template <class T>
double Area(Figure<T>& fig){
	return abs(fig.p[0].first * fig.p[1].second + fig.p[1].first * fig.p[2].second + fig.p[2].first * fig.p[3].second + fig.p[3].first * fig.p[0].second - fig.p[1].first * fig.p[0].second - fig.p[2].first * fig.p[1].second - fig.p[3].first * fig.p[2].second - fig.p[0].first * fig.p[3].second)/2;
}

//-----------------------------------------------------------------------------------------------------------
template <class T>
std::ostream& operator << (std::ostream& os, const std::pair<T, T>& p)
{
    os << '(' << p.first << " , " << p.second << ')';
    return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, Figure<T>& fig)
{
    if (fig.Abstract)
        os << "Abstract: ";
    else if(fig.Trapezoid)
        os << "Trapezoid: ";
    else if(fig.Rectangle)
        os << "Rectangle: ";
    else if(fig.Square)
        os << "Square: ";
    os << fig.p[0] << "; " << fig.p[1] << "; " << fig.p[2] << "; " << fig.p[3] << "; Centre = " << fig.centre << "; Area = " << fig.area << std::endl;
}

//-----------------------------------------------------------------------------------------------------------
bool operator == (std::pair<int, int> lhs, std::pair<int, int> rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool operator == (std::pair<double, double> lhs, std::pair<double, double> rhs)
{
    return DoubleEqual(lhs.first, rhs.first) && DoubleEqual(lhs.second, rhs.second);
}

template <class T>
std::istream& operator >> (std::istream& is, Figure<T>& fig)
{
    std::cout << "Input 4 points A,B,C,D in strict order \nA(x y): \n";
    std::cout << "\tx: ";
    is >> fig.p[0].first;
    std::cout << " \ty: ";
    is >> fig.p[0].second;
    std::cout << "B(x y): \n";
    std::cout << "\tx: ";
    is >> fig.p[1].first;
    std::cout << " \ty: ";
    is >> fig.p[1].second;
    std::cout << "C(x y): \n";
    std::cout <<  "\tx: ";
    is >> fig.p[2].first;
    std::cout << " \ty: ";
    is >> fig.p[2].second;
    std::cout << "D(x y): \n";
    std::cout << "\tx: ";
    is >> fig.p[3].first;
    std::cout << " \ty: ";
    is >> fig.p[3].second;

    if(IsSquare(fig.p))
    	fig.Square = true;
    else if(IsRectangle(fig.p))
    	fig.Rectangle = true;
    else if(IsTrapezoid(fig.p))
    	fig.Trapezoid = true;
    else 
        fig.Abstract = true;

    fig.centre = Centre(fig);
    fig.area = Area(fig);
    return is;
}

#endif