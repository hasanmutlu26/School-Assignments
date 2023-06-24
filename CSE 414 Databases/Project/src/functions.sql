

DELIMITER //

CREATE FUNCTION calculate_total_price(order_id INT) RETURNS NUMERIC(10, 2)
DETERMINISTIC
BEGIN
    DECLARE total_price NUMERIC(10, 2);

    SELECT SUM(op.product_quantity * p.product_price)
    INTO total_price
    FROM ordered_product op
    INNER JOIN product p ON op.product_id = p.product_id
    WHERE op.orders_ID = order_id;

    RETURN total_price;
END//

DELIMITER ;

DELIMITER //

CREATE FUNCTION calculate_total_revenue() RETURNS NUMERIC(10, 2)
READS SQL DATA
DETERMINISTIC
BEGIN
    DECLARE total_revenue NUMERIC(10, 2);

    SELECT SUM(op.product_quantity * p.product_price)
    INTO total_revenue
    FROM ordered_product op
    INNER JOIN product p ON op.product_id = p.product_id;

    RETURN total_revenue;
END//
READS SQL DATA
DETERMINISTIC

DELIMITER ;
