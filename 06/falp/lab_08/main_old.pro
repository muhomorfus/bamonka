domains
	name, phone = symbol.
	city, street = symbol.
	model, color, number = symbol.
	building, appartment = integer.
	price = integer.
	address = address(city, street, building, appartment).

predicates
	nondeterm phone_book(name, phone, address).
	nondeterm car(name, model, color, price, number).
	nondeterm info_by_car(model, color, name, city, phone).

clauses
	phone_book("Tomas", "123-456", address("London", "Temza", 3, 1)).
	phone_book("Tomas", "123-450", address("Paris", "Blabla", 2, 55)).
	phone_book("John", "123-111", address("London", "Temza", 333, 61)).
	phone_book("Artur", "123-000", address("Moskow", "Tverskaya", 15, 112)).
	
	car("Tomas", "Bentley", "black", 1200, "AE222K").
	car("Tomas", "Rolce Royce", "pink", 1500, "AE341C").
	car("Artur", "Bentley", "black", 1000, "AT111K").
	car("Vasya", "Zhigulie", "white", 3000, "EK228X").
	
	info_by_car(Model, Color, Name, City, Phone) :- car(Name, Model, Color, _, _), phone_book(Name, Phone, address(City, _, _, _)).
	
goal
	%car("Tomas", Model, Color, Price, Number).
	%car("Tomas", "Bentley", "black", _, Number).
	%phone_book(Name, Phone, address("London", Street, House, Room)).
	%info_by_car("Bentley", "black", Name, City, Phone).
	phone_book(_, "123-456", address(City, _, _, _)).