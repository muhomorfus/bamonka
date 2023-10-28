domains
    surname, name, phone = symbol.
    city, street = symbol.
    color, number = symbol.
    building, appartment = integer.
    floors, size = integer.
    price = integer.
    sum = integer.
    bank_name = symbol.
    account = symbol.
    address = address(city, street, building, appartment).

    property = car(name, color, number, price);
               building(name, floors, price);
               land(name, size, price);
               water(name, color, number, price).
 
predicates
    nondeterm contact(surname, phone, address).
    nondeterm owner(surname, property).
    nondeterm bank(surname, bank_name, account, sum).

    nondeterm prop_names(surname, name).
    nondeterm prop_prices(surname, name, price).

    nondeterm car_price(surname, price).
    nondeterm building_price(surname, price).
    nondeterm land_price(surname, price).
    nondeterm water_price(surname, price).
    nondeterm sum_price(surname, price).

clauses
    contact("Thomas", "000-001", address("Erevan", "Manhatan", 1, 2)).
    contact("Artur", "000-002", address("London", "Kr. Armii", 3, 4)).
    contact("Thomas", "000-003", address("Paris", "Piva", 5, 6)).
    contact("John", "000-004", address("Moscow", "Mira", 7, 8)).
    contact("Mike", "000-005", address("Minsk", "Pobody", 9, 10)).
 
    owner("Thomas", car("Lada", "black", "pbd001", 5000)).
    owner("John", car("Bentley", "yellow", "pbd002", 10000)).
    owner("John", water("Yacht", "brown", "pbd003", 4000)).
    owner("Mike", building("House", 57, 5000)).
    owner("Artur", land("Hotel", 500, 4000)).
    owner("Artur", building("Bar", 4, 5000)).
    owner("Artur", car("Bentley", "brown", "GGG777", 10000)).

    bank("Thomas", "Sber", "debit", 5000).
    bank("John", "Tinkoff", "credit", 10000).
    bank("John", "Sovkombank", "credit", 4000).
    bank("Mike", "Sber", "debit", 5000).
    bank("Artur", "Sovkombank", "credit", 4000).
 
    prop_names(Surname, Name) :- owner(Surname, car(Name, _, _, _)); 
    				             owner(Surname, building(Name, _, _)); 
    				             owner(Surname, land(Name, _, _)); 
    				             owner(Surname, water(Name, _, _, _)).
 
    prop_prices(Surname, Name, Price) :- owner(Surname, car(Name, _, _, Price)); 
    				                     owner(Surname, building(Name, _, Price)); 
    				                     owner(Surname, land(Name, _, Price));
    				                     owner(Surname, water(Name, _, _, Price)).
 				  
    car_price(Surname, Price) :- owner(Surname, car(_, _, _, Price)), !; Price = 0.
    building_price(Surname, Price) :- owner(Surname, building(_, _, Price)), !; Price = 0.
    land_price(Surname, Price) :- owner(Surname, land(_, _, Price)), !; Price = 0.
    water_price(Surname, Price) :- owner(Surname, water(_, _, _, Price)), !; Price = 0.
 				  
    sum_price(Surname, S) :- car_price(Surname, Pc),
    				         building_price(Surname, Pb),
    				         land_price(Surname, Pl),
    				         water_price(Surname, Pw),
    				         S = Pc + Pb + Pl + Pw.
 
goal
    %prop_names("Artur", Name).
    %prop_prices("John", Name, Price).
    sum_price("John", SumPrice).

  
  