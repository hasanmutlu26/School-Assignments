
CREATE TABLE person (
    person_ID INT AUTO_INCREMENT PRIMARY KEY,
    person_name VARCHAR(25) NOT NULL,
    person_surname VARCHAR(25) NOT NULL
);

CREATE TABLE customer (
    customer_ID INT AUTO_INCREMENT PRIMARY KEY,
    customer_name VARCHAR(25),
    customer_surname VARCHAR(25),
    customer_phone VARCHAR(25) NOT NULL,
    email VARCHAR(60) NOT NULL,
    FOREIGN KEY (customer_ID) REFERENCES person(person_ID) ON DELETE RESTRICT
);

CREATE TABLE customer_address (
    address_ID INT AUTO_INCREMENT PRIMARY KEY,
    customer_ID INT,
    address_text VARCHAR(200) NOT NULL,
    FOREIGN KEY (customer_ID) REFERENCES customer(customer_ID) ON DELETE RESTRICT
);

CREATE TABLE employee (
    employee_ID INT AUTO_INCREMENT PRIMARY KEY,
    employee_name VARCHAR(25),
    employee_surname VARCHAR(25),
    FOREIGN KEY (employee_ID) REFERENCES person(person_ID) ON DELETE RESTRICT
);

CREATE TABLE orders (
    orders_ID INT AUTO_INCREMENT PRIMARY KEY,
    orders_Status VARCHAR(20) NOT NULL,
    request_date DATE NOT NULL
);

CREATE TABLE placed_order (
       orders_ID INT,
       customer_ID INT,
       FOREIGN KEY (orders_ID) REFERENCES orders(orders_ID) ON DELETE RESTRICT,
       FOREIGN KEY (customer_ID) REFERENCES customer(customer_ID) ON DELETE RESTRICT  
);

CREATE TABLE order_prepare (
    orders_ID INT,
    employee_ID INT,
    FOREIGN KEY (orders_ID) REFERENCES orders(orders_ID) ON DELETE RESTRICT,
    FOREIGN KEY (employee_ID) REFERENCES employee(employee_ID) ON DELETE RESTRICT
);

CREATE TABLE product (
    product_id INT AUTO_INCREMENT PRIMARY KEY,
    product_name VARCHAR(50),
    product_description VARCHAR(1000),
    product_price NUMERIC(10, 2) NOT NULL,
    product_rating NUMERIC(4, 2)
);

CREATE TABLE ordered_product (
    product_id INT,
    orders_ID INT,
    product_quantity INT NOT NULL,
    FOREIGN KEY (product_id) REFERENCES product(product_id) ON DELETE RESTRICT,
    FOREIGN KEY (orders_ID) REFERENCES orders(orders_ID) ON DELETE RESTRICT
);

CREATE TABLE payment (
    payment_ID INT AUTO_INCREMENT PRIMARY KEY,
    payment_date DATE NOT NULL,
    credit_card_num VARCHAR(20) NOT NULL,
    name_on_card VARCHAR(25) NOT NULL,
    expiry_month INT NOT NULL,
    expiry_year INT NOT NULL,
    cvv_num INT NOT NULL
);

CREATE TABLE orders_payment (
    payment_ID INT,
    orders_ID INT,
    FOREIGN KEY (payment_ID) REFERENCES payment(payment_ID) ON DELETE RESTRICT,
    FOREIGN KEY (orders_ID) REFERENCES orders(orders_ID) ON DELETE RESTRICT
);

CREATE TABLE reviews (
    review_ID INT AUTO_INCREMENT PRIMARY KEY,
    customer_ID INT,
    product_id INT,
    review_rating NUMERIC(4, 2) NOT NULL,
    review_description VARCHAR(300),
    review_date TIMESTAMP,
    FOREIGN KEY (customer_ID) REFERENCES customer(customer_ID) ON DELETE RESTRICT,
    FOREIGN KEY (product_id) REFERENCES product(product_id) ON DELETE RESTRICT
);



CREATE TABLE delivery (
    delivery_ID INT AUTO_INCREMENT PRIMARY KEY,
    delivery_startdate TIMESTAMP NOT NULL,
    address_ID INT,
    FOREIGN KEY (address_ID) REFERENCES customer_address(address_ID) ON DELETE RESTRICT
);

CREATE TABLE order_delivery(
       orders_id INT,
       delivery_ID INT,
       FOREIGN KEY (delivery_ID) REFERENCES delivery(delivery_ID) ON DELETE RESTRICT,
       FOREIGN KEY (orders_ID) REFERENCES orders(orders_ID) ON DELETE RESTRICT

);

CREATE TABLE delivery_status (
    delivery_status_ID INT AUTO_INCREMENT PRIMARY KEY,
    delivery_ID INT,
    delivery_personel_ID INT,
    delivery_station_name VARCHAR(50) NOT NULL,
    station_arrival_time TIMESTAMP NOT NULL,
    FOREIGN KEY (delivery_ID) REFERENCES delivery(delivery_ID) ON DELETE RESTRICT
);

COMMIT;
