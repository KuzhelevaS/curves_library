#define _USE_MATH_DEFINES

#include "curves.h"

#include <cmath>
#include <string>
#include <stdexcept>

namespace curves {
	Circle::Circle(double radius) : radius_(radius) {
		if (radius < 0) {
			throw std::domain_error(
				std::string("Circle radius mast be positive. The current one is ")
				+ std::to_string(radius));
		}
	}

	Point3D Circle::GetPoint(double t) const {
		return {
			radius_ * std::cos(t),
			radius_ * std::sin(t),
			0};
	}

	Vector3D Circle::GetDerivative(double t) const {
		return {
			-1* radius_ * std::sin(t),
			radius_ * std::cos(t),
			0};
	}

	double Circle::GetRadius() const {
		return radius_;
	}

	Ellipse::Ellipse(double radius_x, double radius_y)
		: radius_x_(radius_x), radius_y_(radius_y)
	{
		if (radius_x < 0 || radius_y < 0) {
			throw std::domain_error(
				std::string("Ellipse radii mast be positive. The current radius_x is ")
				+ std::to_string(radius_x)
				+std::string(". The current radius_y is ")
				+ std::to_string(radius_y));
		}
	}

	Point3D Ellipse::GetPoint(double t) const {
		return {
			radius_x_ * std::cos(t),
			radius_y_ * std::sin(t),
			0 };
	}

	Vector3D Ellipse::GetDerivative(double t) const {
		return {
			-1 * radius_x_ * std::sin(t),
			radius_y_ * std::cos(t),
			0 };
	}

	double Ellipse::GetRadiusX() const {
		return radius_x_;
	}

	double Ellipse::GetRadiusY() const {
		return radius_y_;
	}

	Helix::Helix(double radius, double step)
		: radius_(radius), step_(step)
	{
		if (radius < 0) {
			throw std::domain_error(
				std::string("Helix radius mast be positive. The current one is ")
				+ std::to_string(radius));
		}
	}

	Point3D Helix::GetPoint(double t) const {
		return {
			radius_ * std::cos(t),
			radius_ * std::sin(t),
			step_ * t / (2 * M_PI)};
	}

	Vector3D Helix::GetDerivative(double t) const {
		return {
			-1 * radius_ * std::sin(t),
			radius_ * std::cos(t),
			step_ / (2 * M_PI)};
	}

	double Helix::GetRadius() const {
		return radius_;
	}

	double Helix::GetStep() const {
		return step_;
	}
}
