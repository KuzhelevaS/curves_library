#pragma once

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    #ifdef CURVES_DLL
        #ifdef __GNUC__
            #define CURVES_EXPORT __attribute__ ((dllexport))
        #else
            #define CURVES_EXPORT __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define CURVES_EXPORT __attribute__ ((dllimport))
        #else
            #define CURVES_EXPORT __declspec(dllimport)
        #endif
    #endif
#else
    #define CURVES_EXPORT __attribute__ ((visibility ("default")))
#endif

#include <iostream>
#include <memory>

namespace curves {
	struct CURVES_EXPORT Point3D {
		double x = 0;
		double y = 0;
		double z = 0;
	};

	struct CURVES_EXPORT Vector3D {
		double x = 0;
		double y = 0;
		double z = 0;
	};

	class CURVES_EXPORT Curve {
	public:
		Curve() = default;
		virtual ~Curve() = default;

		virtual Point3D GetPoint(double t) const = 0;
		virtual Vector3D GetDerivative(double t) const = 0;
	};

	class CURVES_EXPORT Circle : public Curve {
	public:
		explicit Circle(double radius);

		Point3D GetPoint(double t) const override;
		Vector3D GetDerivative(double t) const override;
		double GetRadius() const;

	private:
		double radius_;
	};

	class CURVES_EXPORT Ellipse : public Curve {
	public:
		explicit Ellipse(double radius_x, double radius_y);

		Point3D GetPoint(double t) const override;
		Vector3D GetDerivative(double t) const override;
		double GetRadiusX() const;
		double GetRadiusY() const;

	private:
		double radius_x_;
		double radius_y_;
	};

	class CURVES_EXPORT Helix : public Curve {
	public:
		explicit Helix(double radius, double step);

		Point3D GetPoint(double t) const override;
		Vector3D GetDerivative(double t) const override;
		double GetRadius() const;
		double GetStep() const;

	private:
		double radius_;
		double step_;
	};
}
