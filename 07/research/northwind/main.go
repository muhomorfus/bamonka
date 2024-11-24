package main

import (
	"fmt"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

func main() {
	db, err := gorm.Open(postgres.Open("host=localhost port=55432 user=postgres password=postgres dbname=northwind sslmode=disable"), &gorm.Config{})
	if err != nil {
		panic("failed to connect database")
	}

	type Customer struct {
		ContactName string `gorm:"contact_name"`
	}
	var customer Customer
	db.Table("customers c").Select("c.contact_name").Joins("join employees e on c.city = e.city").Joins("join orders o on e.employee_id = o.employee_id").Joins("join order_details od on o.order_id = od.order_id").Where("od.quantity = ?", 91).First(&customer)

	fmt.Println(customer)
}
