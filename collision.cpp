#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
using namespace sf;

double d_r(double degrees);
double v1_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v1_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v2_f_x(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double v2_f_y(double v1, double v2, double m1, double m2, double theta1, double theta2, double phi);
double phi(int x1, int x2, int y1, int y2);

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
	const int a = 50;
	RenderWindow window(VideoMode(window_x, window_y), "Simulation");
	window.setFramerateLimit(30); 
	const double dt = 0.0333; //duration of one frame
	double t = 0; //clock
	//creating object1
	RectangleShape object1(Vector2f(a, a));
	double x1 = 0; double y1 = window_y - a;
	object1.setPosition(Vector2f(x1, y1)); 
	object1.setFillColor(Color::Green);
	double v1 = 0; double v1_x = 0; double v1_y = 0;
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
	double v2 = 0; double v2_x = 0; double v2_y = 0;
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
	bool fire = false;
	bool hit = false;
	bool on_obj1 = false;
	bool on_obj2 = false;
	bool read_once = true;
	int rim_hit1 = 0; //let's mark boundaries as fallows: 0 - no boundary, 1 - up, 2 - right, 3 - down, 4 - left
	int rim_hit2 = 0;
	std::cout << "****Elastic collision simulator****\nYou can specify masses, initial velocities\n"
		<< "and trajcetories of two projectiles.\n Click on an object with mouse and then using arrows\n"
		<< "change its position and direction of initial velocity vector.\n"
		<< "To learn more visit https://github.com/AleksyBalazinski/Elastic-collision" << ".\nEnjoy!";
	std::cout << "\n\nEnter initial velocity for object 1 (green)\n";
	std::cin >> v1;
	std::cout << "\nEnter initial velocity for object 2 (red)\n";
	std::cin >> v2;
	std::cout << "\nEnter mass of object 1\n";
	std::cin >> m1;
	std::cout << "\nEnter mass of object 2\n";
	std::cin >> m2;

	//font
	Font font;
	font.loadFromFile("consola.ttf"); //your font
	Text desc1;
	desc1.setFont(font);
	std::string v1_str = std::to_string(static_cast<int>(v1));
	std::string m1_str = std::to_string(static_cast<int>(m1));
	desc1.setString("v = " + v1_str + "\n" + "m = " + m1_str);
	desc1.setCharacterSize(30);
	desc1.setPosition(0, 5);
	desc1.setFillColor(Color::Green);
	desc1.setOutlineColor(Color::White);
	desc1.setOutlineThickness(1);
	Text desc2;
	desc2.setFont(font);
	std::string v2_str = std::to_string(static_cast<int>(v2));
	std::string m2_str = std::to_string(static_cast<int>(m2));
	desc2.setString("v = " + v2_str + "\n" + "m = " + m2_str);
	desc2.setCharacterSize(30);
	desc2.setPosition(window_x - 120, 5);
	desc2.setFillColor(Color::Red);
	desc2.setOutlineColor(Color::White);
	desc2.setOutlineThickness(1);
	

	while (window.isOpen())
	{
		Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed) window.close();
			if (evt.type == Event::MouseButtonPressed && start == false)
			{
				if (evt.mouseButton.button == Mouse::Left)
				{
					if (evt.mouseButton.x >= object1.getPosition().x && evt.mouseButton.x <= object1.getPosition().x + a
						&& evt.mouseButton.y >= object1.getPosition().y && evt.mouseButton.y <= object1.getPosition().y + a)
					{
						on_obj1 = true; //mouse was left-clicked when cursor was on object1
						on_obj2 = false;
					}
					else if(evt.mouseButton.x >= object2.getPosition().x && evt.mouseButton.x <= object2.getPosition().x + a
						&& evt.mouseButton.y >= object2.getPosition().y && evt.mouseButton.y <= object2.getPosition().y + a)
					{
						on_obj2 = true; //mouse was left-clicked when cursor was on object2
						on_obj1 = false;
					}
					else
					{
						on_obj1 = false;
						on_obj2 = false;
					}
				}
			}
			if (evt.type == Event::KeyPressed)
			{
				if (evt.key.code == Keyboard::Escape) window.close();
				
				if (evt.key.code == Keyboard::Up && start == false) //modify iff spacebar hasn't been pressed
				{
					if (on_obj1 == true)
					{
						object1.move(0, -5);
						line1.move(0, -5);
					}
					if (on_obj2 == true)
					{
						object2.move(0, -5);
						line2.move(0, -5);
					}
				}
				if (evt.key.code == Keyboard::Down && start == false)
				{
					if (on_obj1 == true)
					{
						object1.move(0, 5);
						line1.move(0, 5);
					}
					if (on_obj2 == true)
					{
						object2.move(0, 5);
						line2.move(0, 5);
					}
				}
				if (evt.key.code == Keyboard::Right && start == false)
				{
					if (on_obj1 == true)
					{
						line1.rotate(1);
					}
					if (on_obj2 == true)
					{
						line2.rotate(1);
					}
				}
				if (evt.key.code == Keyboard::Left && start == false)
				{
					if (on_obj1 == true)
					{
						line1.rotate(-1);
					}
					if (on_obj2 == true)
					{
						line2.rotate(-1);
					}
				}
				if (evt.key.code == Keyboard::Space)
				{
					start = true; //start of the simulation
					fire = true; 
					read_once = false;
				}
			}
		}
		if (read_once == false) // read position ob object1 & object2 immediately after spacebar has been pressed
		{
			x1 = object1.getPosition().x;
			y1 = object1.getPosition().y;
			angle1 = line1.getRotation();

			x2 = object2.getPosition().x;
			y2 = object2.getPosition().y;
			angle2 = line2.getRotation();

			t = 0; //reset clock
			read_once = true;
		}
		if (fire == true) //velocities after hitting spacebar (motion before hitting)
		{
			v1_x = v1 * cos(d_r(angle1));
			v1_y = v1 * sin(d_r(angle1));

			v2_x = v2 * cos(d_r(angle2));
			v2_y = v2 * sin(d_r(angle2));

			fire = false;
		}
		
		if (hit == true) //velocities after collision
		{
			v1_x = v1_f_x(v1, v2, m1, m2, d_r(angle1), d_r(angle2), phi(x1, x2, y1, y2));
			v1_y = v1_f_y(v1, v2, m1, m2, d_r(angle1), d_r(angle2), phi(x1, x2, y1, y2));

			v2_x = v2_f_x(v1, v2, m1, m2, d_r(angle1), d_r(angle2), phi(x1, x2, y1, y2));
			v2_y = v2_f_y(v1, v2, m1, m2, d_r(angle1), d_r(angle2), phi(x1, x2, y1, y2));
		}

		if (rim_hit1 == 1 || rim_hit1 == 3)
		{
			v1_y = (-1)*v1_y;
			rim_hit1 = 0;
			std::cout << "\ncollsion with the boundary 1 or 3!";
			hit = false;
		}
		if (rim_hit1 == 2 || rim_hit1 == 4)
		{
			v1_x = (-1) * v1_x;
			rim_hit1 = 0;
			std::cout << "\ncollsion with the boundary 2 or 4!";
			hit = false;
		}
		if (rim_hit2 == 1 || rim_hit2 == 3)
		{
			v2_y = (-1) * v2_y;
			rim_hit2 = 0;
			std::cout << "\ncollsion with the boundary 1 or 3!";
			hit = false;
		}
		if (rim_hit2 == 2 || rim_hit2 == 4)
		{
			v2_x = (-1) * v2_x;
			rim_hit2 = 0;
			std::cout << "\ncollsion with the boundary 2 or 4!";
			hit = false;
		}

		if (start == true)
		{
			t += dt; //it's very important to add dt BEFORE moving objects to a new position, otherwise they'll be stuck here for dt

			object1.setPosition(x1 + v1_x * t, y1 + v1_y * t);
			object2.setPosition(x2 + v2_x * t, y2 + v2_y * t);
		}
		//collision detection
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
			std::cout << "\ncollision with another object!\n";
			hit = true;
			read_once = false;
		}
		//boundary hit: to be done in a next version
		if ((object1.getPosition().y <= 0) && start == true) { rim_hit1 = 1; read_once = false; }
		if ((object1.getPosition().x + a >= window_x) && start == true) { rim_hit1 = 2;  read_once = false; }
		if ((object1.getPosition().y + a >= window_y) && start == true) { rim_hit1 = 3;  read_once = false; }
		if ((object1.getPosition().x <= 0) && start == true) { rim_hit1 = 4;  read_once = false; }

		if ((object2.getPosition().y <= 0) && start == true) { rim_hit2 = 1;  read_once = false; }
		if ((object2.getPosition().x + a >= window_x) && start == true) { rim_hit2 = 2;  read_once = false; }
		if ((object2.getPosition().y + a >= window_y) && start == true) { rim_hit2 = 3;  read_once = false; }
		if ((object2.getPosition().x <= 0) && start == true) { rim_hit2 = 4;  read_once = false; }

		
		//draw stuff
		window.clear();
		if (start == false)
		{
			window.draw(line1);
			window.draw(line2);
		}
		window.draw(object1);
		window.draw(object2);
		window.draw(desc1);
		window.draw(desc2);
		//display everything
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
double phi(int x1, int x2, int y1, int y2)
{
	return atan((y2 - y1) / (x2 - x1));
}
