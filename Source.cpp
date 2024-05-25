/* NUCES Airline Flight System (NAFS) */
/* Miral Hasan 22i-8753 SE-D */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

const int MAX_USERS = 100;

class User {
protected:
	string username;
	string password;
	User* users[MAX_USERS] = { nullptr }; // Array of User pointers
	int numUsers = 0; // Number of users
	string accountNumber;
public:
	User()
	{
		username = "";
		password = "";
	}
	User(string u, string p)
	{
		username = u;
		password = p;
	}
	User(const User& u) 
	{
		password = u.password;
		username = u.username;
	}
	void setUsername(string u)
	{
		username = u;
	}
	string getUsername() const
	{
		return username;
	}
	void setPassword(string p)
	{
		password = p;
	}
	string getPassword() const
	{
		return password;
	}

	// Function to validate username
	bool isValidUsername(const string& username)
	{
		// Username should only contain alphanumeric characters
		for (int i = 0; i < username.length(); i++)
		{
			char c = username[i];
			if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9')) {
				return false;
			}
		}
		return true;
	}

	bool isValidPassword(const string& password)
	{
		// Password should be at least 8 characters long and contain at least one of each: digit, uppercase letter, lowercase letter, and special character
		if (password.length() < 8)
		{
			return false;
		}

		bool hasDigit = false;
		bool hasUppercase = false;
		bool hasLowercase = false;
		bool hasSpecial = false;

		for (int i = 0; i < password.length(); i++)
		{
			char c = password[i];
			if (c >= '0' && c <= '9')
			{
				hasDigit = true;
			}
			else if (c >= 'A' && c <= 'Z')
			{
				hasUppercase = true;
			}
			else if (c >= 'a' && c <= 'z')
			{
				hasLowercase = true;
			}
			else
			{
				hasSpecial = true;
			}
		}

		return hasDigit && hasUppercase && hasLowercase && hasSpecial;
	}



	void registerUser()
	{
		string username;
		string password;

		cout << "Enter a username: ";
		cin >> username;

		if (!isValidUsername(username))
		{
			cout << "Invalid username." << endl;
			return;
		}

		cout << "Enter a password: ";
		cin >> password;

		if (!isValidPassword(password))
		{
			cout << "Invalid password." << endl;
			return;
		}

		// Open the user file in append mode
		ofstream userFile("users.txt", ios::app);

		if (!userFile)
		{
			cout << "Error: could not open user file." << endl;
			return;
		}

		// Write the username and password to the file
		userFile << username << " " << password << endl;

		cout << "User registered successfully." << endl;

		// Close the file
		userFile.close();
	}

	void loginUser()
	{
		string username;
		string password;
		string storedUsername;
		string storedPassword;
		cout << "Enter your username: ";
		cin >> username;

		cout << "Enter your password: ";
		cin >> password;

		// Open the user file in input mode
		ifstream userFile("users.txt");

		if (!userFile) {
			cout << "Error: could not open user file." << endl;
			return;
		}

		// Loop through the file to find the matching username and password
		while (userFile >> storedUsername >> storedPassword)
		{
			if (username == storedUsername && password == storedPassword) 
			{
				cout << "Login successful." << endl;
				userFile.close();
				return;
			}
		}

		cout << "Invalid username or password." << endl;

		// Close the file
		userFile.close();
	}
};
static const int MAX_ROUTES = 10;
class Admin : public User {
private:
	string routes[MAX_ROUTES];
	int numRoutes;
	int maxPassengers;
	Admin** admins; // Dynamic array of pointers to Admin objects
	int numAdmins;

public:
	Admin()
	{

	}
	Admin(string u, string p, int numroutes, int maxpassengers) : User(u, p)
	{
		username = u;
		password = p;
		numRoutes = numroutes;
		maxPassengers = maxpassengers;
		numAdmins = 0;
		admins = new Admin * [0];
	}

	void registerAdmin()
	{
		string u, p;
		cout << "Enter the new admin's username: ";
		cin >> u;
		cout << "Enter the new admin's password: ";
		cin >> p;

		Admin** newAdmins = new Admin * [numAdmins + 1];
		for (int i = 0; i < numAdmins; i++)
		{
			newAdmins[i] = admins[i];
		}
		newAdmins[numAdmins] = new Admin(u, p, numRoutes, maxPassengers);
		delete[] admins;
		admins = newAdmins;
		numAdmins++;
		cout << "Admin registered successfully." << endl;
	}


	void addRoute(string route, string filename)
	{
		ifstream infile(filename);
		if (!infile.is_open())
		{
			cout << "Error: Could not open file: " << filename << endl;
			return;
		}

		// To check if the route already exists in the file
		string line;
		bool routeExists = false;
		while (getline(infile, line))
		{
			if (line == route)
			{
				cout << "Route already exists." << endl;
				routeExists = true;
				break;
			}
		}
		if (routeExists)
		{
			infile.close();
			return;
		}

		infile.close();

		// Append the new route to the file
		ofstream outfile(filename, ios_base::app);
		if (!outfile.is_open())
		{
			cout << "Error: Could not open file: " << filename << endl;
			return;
		}

		outfile << route << endl;
		cout << "Route added successfully." << endl;

		outfile.close();
	}



	void removeRoute(string route)
	{
		ifstream infile("routes.txt");
		if (!infile.is_open()) {
			cout << "Error opening file." << endl;
			return;
		}

		ofstream outfile("temp.txt");
		if (!outfile.is_open())
		{
			cout << "Error creating temp file." << endl;
			infile.close();
			return;
		}

		// Copy all lines except the one to be removed to the temp file
		string line;
		bool routeFound = false;
		while (getline(infile, line))
		{
			if (line == route) 
			{
				routeFound = true;
			}
			else {
				outfile << line << endl;
			}
		}
		infile.close();
		outfile.close();

		if (!routeFound) {
			cout << "Route not found." << endl;
			remove("temp.txt");  // delete temp file
			return;
		}

		// Replace the original file with the temp file
		remove("routes.txt");
		rename("temp.txt", "routes.txt");

		cout << "Route removed successfully." << endl;
	}

	void setMaxPassengers(int max)
	{
		maxPassengers = max;
	}

	void updateAirlineInquiryDetails() 
	{
		maxPassengers;
		numRoutes;
	}
};



class Passenger : public User {
private:
	string cnic;
	string passport;
	bool local_citizen;
	bool visa_stamp;
public:
	Passenger()
	{
		cnic = "";
		visa_stamp = false;
		passport = "";
		local_citizen = true;
	}
	Passenger(string u, string p, string cnic, string v, string passport, bool citizen) : User(u, p)
	{
		cnic = cnic;
		visa_stamp = false;
		passport = passport;
		local_citizen = citizen;
	}
	void setCNIC(string cnic)
	{
		this->cnic = cnic;
	}
	string getCNIC()
	{
		return cnic;
	}
	void setVisaStamp(bool visa)
	{
		this->visa_stamp = visa;
	}
	bool hasVisaStamp()
	{
		return visa_stamp;
	}
	void setPassport(string passport)
	{
		this->passport = passport;
	}
	string getPassport()
	{
		return passport;
	}
	void setLocalCitizen(bool citizen)
	{
		this->local_citizen = citizen;
	}

};

class LocalPassenger : public Passenger {
public:
	LocalPassenger() : Passenger() 
	{
		setVisaStamp(true); // Local passengers don't need VISA stamp
		setLocalCitizen(true);
	}
};
class ForeignPassenger : public Passenger {
public:
	ForeignPassenger() : Passenger()
	{
		setVisaStamp(false); // Foreign passengers need VISA stamp
		setLocalCitizen(false);
	}
};

class Airport {
protected:
	string names[5];
	string departure_airport;
	string destination_airport;
public:
	Airport()
	{
		departure_airport = "";
		destination_airport = "";
	}
	Airport(string name, string dep_airport, string dest_airport)
	{
		name = name;
		departure_airport = dep_airport;
		destination_airport = dest_airport;
	}
	void setName(string arr[5], int num)
	{
		for (int i = 0; i < 5; i++) {
			names[i] = arr[i];
		}
	}
	string getName(int i)
	{
		return names[i];
	}
	void set_departure_airport(string dep_airport) 
	{
		departure_airport = dep_airport;
	}
	string get_departure_airport() 
	{
		return departure_airport;
	}
	void set_destination_airport(string dest_airport) 
	{
		destination_airport = dest_airport;
	}
	string get_destination_airport() 
	{
		return destination_airport;
	}
	
};
const int flights = 10;
class Flight {
protected:
	string DepartureAirport;
	string DestinationAirport;
	int DepartureTime;
	int ArrivalTime;
	int numof_economyseats;
	int numof_businessseats;
	int priceof_economy;
	int priceof_business;
	int num_flights=5;
	int arrivalTime;
	int departureTime;
	string route;
	string name;
	int flight_number;
	Flight *flights[flights] = { nullptr };
	Airport* departure_airport; // aggregation relationship
	Airport* destination_airport; // aggregation relationship
public:
	Flight()
	{
		DepartureAirport = "Karachi";
		DestinationAirport = "Lahore";
		DepartureTime = 0200;
		ArrivalTime = 0600;
		numof_economyseats = 50;
		numof_businessseats = 10;
		priceof_economy = 25000;
		priceof_business = 35000;
		name = "";
	}
	Flight(string departure, string arrival, int departure_time, int arrival_time, int ecoseats, int busiseats, int ecoprice, int busiprice, string name)
	{
		DepartureAirport = departure;
		DestinationAirport = arrival;
		DepartureTime = departure_time;
		ArrivalTime = arrival_time;
		numof_economyseats = ecoseats;
		numof_businessseats = busiseats;
		priceof_economy = ecoprice;
		priceof_business = busiprice;
		name = name;
	}
	void setDepartureAirport(string departure)
	{
		this->DepartureAirport = departure;
	}
	string getDepartureAirport()
	{
		return DepartureAirport;
	}
	void setDestinationAirport(string arrival)
	{
		this->DestinationAirport = arrival;
	}
	string getDestinationAirport()
	{
		return DestinationAirport;
	}
	void setDepartureTime(int departure_time)
	{
		this->DepartureTime = departure_time;
	}
	int getDepartureTime() 
	{
		return DepartureTime;
	}
	void setArrivalTime(int arrival_time)
	{
		this->ArrivalTime = arrival_time;
	}
	int getArrivalTime()
	{
		return ArrivalTime;
	}
	void setNumOfEconomySeats(int ecoseats)
	{
		this->numof_economyseats = ecoseats;
	}
	int getNumOfEconomySeats()
	{
		return numof_economyseats;
	}
	void setNumOfBusinessSeats(int busiseats)
	{
		this->numof_businessseats = busiseats;
	}
	int getNumberOfBusinessseats()
	{
		return numof_businessseats;
	}
	void setPriceOfEconomy(int ecoprice)
	{
		this->priceof_economy = ecoprice;
	}
	int getPriceOfEconomy()
	{
		return priceof_economy;
	}
	void setPriceOfBusiness(int busiprice)
	{
		this->priceof_business = busiprice;
	}
	int getPriceOfBusiness()
	{
		return priceof_business;
	}

	void updateEconomySeats(int seats)
	{
		numof_economyseats = seats;
	}
	void updateBusinessSeats(int seats)
	{
		numof_businessseats = seats;
	}

	void set_departure_airport(Airport* airport)
	{
		departure_airport = airport;
	}
	void set_destination_airport(Airport* airport)
	{
		destination_airport = airport;
	}
	Airport* get_departure_airport() const
	{
		return departure_airport;
	}
	Airport* get_destination_airport() const 
	{
		return destination_airport;
	}
	int getFlightNumber() const
	{
		return flight_number;
	}
	void setFlightNumber(int flight_number)
	{
		flight_number = flight_number;
	}
	void setRoute(string route)
	{
		this->route = route;
	}
	string getRoute()
	{
		return route;
	}
	void setName(string name)
	{
		this->name = name;
	}
	string getName()
	{
		return name;
	}

	bool is_logged_in()
	{
		return true;
	}


	// Override the display function to include information about the departure and destination airports
	void display() {
		cout << "Flight Information: " << endl;
		cout << "Departure Airport: " << DepartureAirport << endl;
		cout << "Destination Airport: " << DestinationAirport << endl;
		cout << "Departure Time: " << DepartureTime << endl;
		cout << "Arrival Time: " << ArrivalTime << endl;
		cout << "Number of Economy Seats: " << numof_economyseats << endl;
		cout << "Number of Business Seats: " << numof_businessseats << endl;
		cout << "Price of Economy Seat: " << priceof_economy << endl;
		cout << "Price of Business Seat: " << priceof_business << endl;
	}

	void book_flight()
	{
		// Check if user is logged in
		if (!is_logged_in())
		{
			cout << "You must be logged in to book a flight." << endl;
			return;
		}

		// Display available flights
		cout << endl << endl;
		cout << "Available flights:" << endl;
		cout << "1. Karachi to lahore" << endl;
		cout << "2. Lahore to Islamabad" << endl;
		cout << "3. Islamabad to Canada" << endl;
		cout << "4. Peshawar to Karachi" << endl;
		cout << "5. Quetta to Lahore" << endl;

		int choice=5;
		cout <<  endl << "Select a flight: ";
		cin >> choice;
		choice--;
		
		if (choice < 0 || choice >= num_flights)
		{
			cout << "Invalid choice. Please try again." << endl;
			return;
		}

		Flight* selected_flight = flights[choice];
		system("CLS");
		// Display available seat classes and prices
		cout << endl << "Available seat classes and prices for flight "  << ": 60" << endl;
		cout << "1. Economy (50 seats available) - Rs 10,000"  << endl;
		cout << "2. Business (10 seats available) - Rs, 20,000" << endl;
		cout << endl;

		int seat_class;

		cout << endl << "Select a seat class: " << endl << "Press 1 for Economy and 2 for Business" << endl;
		cin >> seat_class;

		if (seat_class < 1 || seat_class > 2)
		{
			cout << "Invalid choice. Please try again." << endl;
			return;
		}

		int num_seats;
		if (seat_class == 1)
		{
			num_seats = 50;
		}
		else
		{
			num_seats = 10;
		}

		cout << endl << "How many seats would you like to book? (max " << num_seats << "): ";
		cin >> num_seats;

		if (num_seats < 1 || num_seats > num_seats)
		{
			cout << "Invalid number of seats. Please try again." << endl;
			return;
		}

		// Book the seats and update the flight object
		if (seat_class == 1)
		{
			50- num_seats;
		}
		else
		{
			10 - num_seats;
		}
		system("CLS");
		cout << "Booking successful. Thank you for choosing our airline!" << endl;
	}

};

class LocalFlight : public Flight {
private:
	int priceof_economy;
	int priceof_business;
public:
	LocalFlight(string departure, string arrival, int departure_time, int arrival_time, int eco_seats, int busi_seats, int eco_price, int busi_price, string name)
		: Flight(departure, arrival, departure_time, arrival_time, eco_seats, busi_seats, eco_price, busi_price, name), priceof_economy(eco_price), priceof_business(busi_price)
	{}

	void setEconomyPrice(int price)
	{
		priceof_economy = price;
	}

	// Override the calculate_total_cost function for a local flight
	int calculate_total_cost(int num_of_passengers, bool is_business_class) {
		int base_price = is_business_class ? priceof_business : priceof_economy;
		int total_cost = base_price * num_of_passengers;
		return total_cost;
	}

	~LocalFlight() {
		cout << "LocalFlight destructor called." << endl;
	}
};


class InternationalFlight : public Flight {
public:
	InternationalFlight(string departure, string arrival, int departure_time, int arrival_time, int ecoseats, int busiseats, int ecoprice, int busiprice, string name) : Flight( departure,  arrival, departure_time, arrival_time, ecoseats, busiseats, ecoprice, busiprice, name)
	{}

	void setBusinessPrice(int price)
	{
		priceof_business = price;
	}
};



void displayBasicInfo()
{
	cout << "**********************************************************************************************************" << endl;
	cout << "                                       Welcome to NAFS!!" << endl;
	cout << endl;
	cout << "         We offer flights to many destinations around the world and within the country." << endl;
	cout << "  Our fleet consists of modern and well-maintained aircraft which provide you the luxury of travelling." << endl;
	cout << "                            You may choose an option below to proceed further:" << endl;
	cout << "**********************************************************************************************************" << endl;
}

void displayGuestMenu() 
{
	cout << "1. Register" << endl;
	cout << "2. Login" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter your choice: ";
}





int main()
{
	int choice;
	int choose;
	char username[50];
	char password[50];
	User user;
	Admin a;
	string route;
	string filename;
	int numroutes = 5;
	int maxpassengers = 60;
	Passenger p;
	Airport A;
	string cityNames[5] = { "lahore", "Karachi", "Islamabad", "Peshawar", "Quetta"};
	A.setName(cityNames, 5);
	Flight F;



	do
	{
		displayBasicInfo();
		cout << endl;
		cout << "                                  1. Register a new passenger" << endl;
		cout << "                                  2. Login " << endl;
		cout << "                                  3. Admin Panel" << endl;
		cout << "                                  4. Book a flight" << endl;
		cout << "                                  5. Exit" << endl;

		cout << "                                  Enter your choice (1-5): ";
		cin >> choice;

		switch (choice) {
		case 1:
			system("CLS");
			// Register a new passenger
			cout << endl << "Your Password must be 8 characters long and use of minimum one special character, uppercase, lowercase, and numeric digit is must." << endl;
			user.registerUser();

			if (user.isValidUsername(username)) {
				cout << "Valid username" << endl;
			}

			if (user.isValidPassword(password)) {
				cout << "Valid password" << endl;
			}
			break;
		case 2:
			system("CLS");
			// login
			user.loginUser();
			break;
		case 3:
			system("CLS");
			cout << endl;
			cout << "1. Register a new admin" << endl;
			cout << "2. Add Route " << endl;
			cout << "3. Remove Route" << endl;
			cout << "4. Update Inquiry" << endl;
			cout << "5. Exit" << endl;
			// Admin Panel
			cout << "Enter your choice (1-5): ";
			cin >> choose;

			switch (choose)
			{
			case 1:
				//register admin
				a.registerAdmin();
				break;

			case 2:
				// add route
				a.addRoute(route, filename);
				break;

			case 3:
				// remove route
				a.removeRoute(route);
				break;

			case 4:
				// update inquiry
				a.updateAirlineInquiryDetails();
				break;

			case 5:
				cout << "Exiting program..." << endl;
				break;

			default:
				cout << "Invalid choice. Please try again." << endl;
				break;
			}

			break;
		case 4:
			system("CLS");
			// book a flight
			F.display();
			F.book_flight();
			break;

		case 5:
			system("CLS");
			cout << "Thank you for using NAFS" << endl;
			cout << "Exiting program..." << endl;
			break;

		default:
			cout << "Invalid choice. Please try again." << endl;
			break;
		}

		cout << endl;

	} while (choice != 6);

	return 0;
}
