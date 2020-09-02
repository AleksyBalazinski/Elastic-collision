#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
using namespace sf;

double d_r(double degrees);
double v1_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v1_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v2_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v2_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);

class point
{
public:
	int x;
	int y;
	void SetNew(int x_pos, int y_pos)
	{
		x = x_pos;
		y = y_pos;
	}
};

int main()
{
	const int window_x = 800;
	const int window_y = 800;
	const int a = 20;
	RenderWindow window(VideoMode(window_x, window_y), "Simulation");
	window.setFramerateLimit(30); 
	const double dt = 0.0333; //duration of one frame
	double t = 0;
	double t_after = 0;
	//creating object1
	RectangleShape object1(Vector2f(a, a));
	double x1 = 0; double y1 = window_y - a;
	object1.setPosition(Vector2f(x1, y1)); 
	object1.setFillColor(Color::Green);
	double v1 = 0;
	double angle1 = 0;
	double m1 = 0;
	//points of reference
	point A{ x1, y1 };
	point B{ x1 + a, y1 };
	point C{ x1 + a, y1 + a };
	point D{ x1, y1 + a };
	//create object2
	RectangleShape object2(Vector2f(a, a));
	double x2 = window_x - a; double y2 = 0;
	object2.setPosition(Vector2f(x2, y2)); 
	object2.setFillColor(Color::Red);
	double v2 = 0;
	double angle2 = 180;
	double m2 = 0;
	//create line1
	RectangleShape line1(Vector2f(window_x * 0.7, 1));
	line1.setPosition(Vector2f(0, window_y - 0.5 * a));
	line1.setFillColor(Color::Green);
	//create line2
	RectangleShape line2(Vector2f(window_x * 0.7, 1));
	line2.setPosition(Vector2f(window_x - a, a/2));
	line2.setRotation(180);
	line2.setFillColor(Color::Red);
	bool start = false;
	bool hit = false;
	std::cout << "Enter initial velocity for object 1 (green)\n";
	std::cin >> v1;
	std::cout << "\nEnter initial velocity for object 2 (red)\n";
	std::cin >> v2;
	std::cout << "\nEnter mass of object 1\n";
	std::cin >> m1;
	std::cout << "\nEnter mass of object 2\n";
	std::cin >> m2;
	while (window.isOpen())
	{
		Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed) window.close();
			if (evt.type == Event::KeyPressed)
			{
				if (evt.key.code == Keyboard::Escape) window.close();
				//move oject1
				if (evt.key.code == Keyboard::Num1 && start == false)
				{
					object1.move(0, -5);
					y1 -= 5;
					line1.move(0, -5);
				}
				if (evt.key.code == Keyboard::Num2 && start == false)
				{
					object1.move(0, 5);
					y1 += 5;
					line1.move(0, 5);
				}
				if (evt.key.code == Keyboard::Num3 && start == false)
				{
					line1.rotate(1);
					angle1 += 1;
				}
				if (evt.key.code == Keyboard::Num4 && start == false)
				{
					line1.rotate(-1);
					angle1 -= 1;
				}
				//move object2
				if (evt.key.code == Keyboard::Num5 && start == false)
				{
					object2.move(0, -5);
					y2 -= 5;
					line2.move(0, -5);
				}
				if (evt.key.code == Keyboard::Num6 && start == false)
				{
					object2.move(0, 5);
					y2 += 5;
					line2.move(0, 5);
				}
				if (evt.key.code == Keyboard::Num7 && start == false)
				{
					line2.rotate(1);
					angle2 += 1;
				}
				if (evt.key.code == Keyboard::Num8 && start == false)
				{
					line2.rotate(-1);
					angle2 -= 1;
				}
				if (evt.key.code == Keyboard::Space)
				{
					start = true;

				}
				//...
			}
		}
		//motion
		
		if (start == true && hit == false) 
		{
			object1.setPosition(x1 + v1 * cos(d_r(angle1)) * t, y1 + v1 * sin(d_r(angle1)) * t);
			object2.setPosition(x2 + v2 * cos(d_r(angle2)) * t, y2 + v2 * sin(d_r(angle2)) * t);
			
			t += dt;
		}

		//collision mechanism
		A.SetNew(object1.getPosition().x, object1.getPosition().y);
		B.SetNew(object1.getPosition().x + a, object1.getPosition().y);
		C.SetNew(object1.getPosition().x + a, object1.getPosition().y + a);
		D.SetNew(object1.getPosition().x, object1.getPosition().y + a);
		int o2_x = object2.getPosition().x;
		int o2_y = object2.getPosition().y;
		if (((A.x >= o2_x && A.x <= o2_x + a && A.y >= o2_y && A.y <= o2_y + a)
			|| (B.x >= o2_x && B.x <= o2_x + a && B.y >= o2_y && B.y <= o2_y + a)
			|| (C.x >= o2_x && C.x <= o2_x + a && C.y >= o2_y && C.y <= o2_y + a)
			|| (D.x >= o2_x && D.x <= o2_x + a && D.y >= o2_y && D.y <= o2_y + a)) && hit == false)
		{
			std::cout << "\ncollision!\n";
			x1 = object1.getPosition().x;
			y1 = object1.getPosition().y;
			x2 = object2.getPosition().x;
			y2 = object2.getPosition().y;
			hit = true;
		}
		//after collision
		if (hit == true)
		{
			object1.setPosition(x1 + v1_f_x(v1,v2,m1,m2,d_r(angle1),d_r(angle2), d_r(angle1))*t_after, //assume m1 >> m2 FIX THAT!!!
				y1 + v1_f_y(v1, v2, m1, m2, d_r(angle1), d_r(angle2), 0)*t_after);
			object2.setPosition(x2 + v2_f_y(v1, v2, m1, m2, d_r(angle1), d_r(angle2), d_r(angle1))*t_after,
				y2 + v2_f_y(v1, v2, m1, m2, d_r(angle1), d_r(angle2), 0)*t_after);

			t_after += dt;
		}

		window.clear();
		window.draw(line1);
		window.draw(line2);
		window.draw(object1);
		window.draw(object2);
		//window.draw(...)
		window.display();
	}
}

double d_r(double degrees)
{
	return ((3.14159 / 180) * degrees);
}

double v1_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi)
{
	return (((v1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2 * v2 * cos(theta2 - phi)) / (m1 + m2)) 
		* cos(phi) + v1 * sin(theta1 - phi) * cos(phi + 3.14159 / 2));
}
double v1_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi)
{
	return (((v1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2 * v2 * cos(theta2 - phi)) / (m1 + m2))
		* sin(phi) + v1 * sin(theta1 - phi) * sin(phi + 3.14159 / 2));
}
double v2_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi)
{
	return (((v2 * cos(theta2 - phi) * (m2 - m1) + 2 * m1 * v1 * cos(theta1 - phi)) / (m1 + m2))
		* cos(phi) + v2 * sin(theta2 - phi) * cos(phi + 3.14159 / 2));
}
double v2_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi)
{
	return (((v2 * cos(theta2 - phi) * (m2 - m1) + 2 * m1 * v1 * cos(theta1 - phi)) / (m1 + m2))
		* sin(phi) + v2 * sin(theta2 - phi) * sin(phi + 3.14159 / 2));
}