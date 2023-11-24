#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <numeric>
#include <future>
#include <cassert>

#include "curves.h"

std::ostream& operator << (std::ostream & out, curves::Point3D p) {
	out << '{' << p.x << ',' << p.y << ',' << p.z << '}';
	return out;
}

std::ostream& operator << (std::ostream & out, curves::Vector3D v) {
	out << '{' << v.x << ',' << v.y << ',' << v.z << '}';
	return out;
}

std::unique_ptr<curves::Curve> GenerateCurve(std::mt19937 & generator) {
	using namespace curves;
	std::uniform_int_distribution<int> random_type(1, 3);
	int curve_type = random_type(generator);

	std::uniform_real_distribution<double> random_radius(0.0, 1'000.0);
	if (curve_type == 1) {
		return std::make_unique<Circle>(Circle(random_radius(generator)));
	} else if (curve_type == 2) {
		return std::make_unique<Ellipse>(Ellipse(random_radius(generator), random_radius(generator)));
	} else {
		std::uniform_real_distribution<double> random_step(0.0, 1'000.0);
		return std::make_unique<Helix>(Helix(random_radius(generator), random_step(generator)));
	}
}

int main() {
	// Random setttings
	std::random_device rd;
	std::mt19937 random_engine(rd());

	// Make container with random curves
	std::uniform_int_distribution<int> random_count(1, 1'000'000);
	std::size_t curves_count = random_count(random_engine);
	std::vector<std::shared_ptr<curves::Curve>> objects;
	objects.reserve(curves_count);

	for (std::size_t i = 0; i < curves_count; ++i) {
		try {
			auto curve_ptr = GenerateCurve(random_engine);
			objects.push_back(std::move(curve_ptr));
		} catch (std::exception & exep) {
			std::cerr << __FILE__ << ", line " << __LINE__ << ": "
				<< exep.what() << std::endl;
		}
	}

	// Print curves info
	double t = M_PI / 4;
	for (const auto & object: objects) {
		std::cout << "3D point: " << object->GetPoint(t)
			<< "; first derivative (3D vector): " << object->GetDerivative(t)
			<< std::endl;
	}

	// Second container with only circles
	std::vector<std::shared_ptr<curves::Circle>> circles;
	circles.reserve(objects.size() / 3);
	for (const auto & object: objects) {
		std::shared_ptr<curves::Circle> circle = std::dynamic_pointer_cast<curves::Circle>(object);
		if (circle) {
			circles.push_back(std::move(circle));
			assert(object.get() == circles.back().get());
		}
	}

	// Sort circles
	std::sort(circles.begin(), circles.end(), [](auto & lhs, auto & rhs){
		return lhs->GetRadius() < rhs->GetRadius();
	});

	std::cout << std::setprecision(12);
	// Get sum of the radii in a single thread
	{
		using namespace std::chrono;
		steady_clock::time_point start_time = steady_clock::now();
		double radius_sum = std::accumulate(circles.begin(), circles.end(), 0.0,
			[](double sum, auto circle){
				return sum + circle->GetRadius();
			});
		steady_clock::time_point end_time = steady_clock::now();
		std::cout << "Sum of the circle radii is " << radius_sum
			<< " in " << duration_cast<microseconds>(end_time - start_time).count() << "μs"
			<< " with single thread" << std::endl;
	}

	// Get sum of the radii with multithreading
	{
		using namespace std::chrono;
		steady_clock::time_point start_time = steady_clock::now();

		unsigned int thread_count = std::thread::hardware_concurrency();
		std::vector<std::future<double>> partial_sums;

		auto GetRangeSum = [](auto begin, auto end){
			double sum = 0;
			for (auto it = begin; it != end; it++) {
				sum += (*it)->GetRadius();
			}
			return sum;
		};

		// increase by 1 to round up
		std::size_t items_per_thread = circles.size() / thread_count + 1;

		auto it = circles.begin();
		while (it < circles.end()) {
			auto next = it + std::min(items_per_thread, static_cast<std::size_t>(circles.end() - it));
			partial_sums.push_back(std::async(GetRangeSum, it, next));
			it = next;
		}

		double radius_sum = 0;
		std::for_each(partial_sums.begin(), partial_sums.end(), [&radius_sum](std::future<double>& part){
			radius_sum += part.get();
		});

		steady_clock::time_point end_time = steady_clock::now();
		std::cout << "Sum of the circle radii is " << radius_sum
			<< " in " << duration_cast<microseconds>(end_time - start_time).count() << "μs"
			<< " with multhithreading"<< std::endl;
	}

}
