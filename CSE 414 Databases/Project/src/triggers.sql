DELIMITER //

CREATE TRIGGER update_product_rating
AFTER INSERT ON reviews
FOR EACH ROW
BEGIN
    DECLARE avg_rating NUMERIC(4,2);
    
    SELECT AVG(review_rating) INTO avg_rating
    FROM reviews
    WHERE product_id = NEW.product_id;
    
    UPDATE product
    SET product_rating = avg_rating
    WHERE product_id = NEW.product_id;
END //


CREATE TRIGGER employee_insert_trigger
BEFORE INSERT ON employee
FOR EACH ROW
BEGIN
    INSERT INTO person (person_ID, person_name, person_surname)
    VALUES (NEW.employee_ID, NEW.employee_name, NEW.employee_surname);
END//

CREATE TRIGGER customer_insert_trigger
BEFORE INSERT ON customer
FOR EACH ROW
BEGIN
    INSERT INTO person (person_ID, person_name, person_surname)
    VALUES (NEW.customer_ID, NEW.customer_name, NEW.customer_surname);
END//

CREATE TRIGGER restrict_review_placement
BEFORE INSERT ON reviews
FOR EACH ROW
BEGIN
    DECLARE order_exists INT;
    
    SELECT COUNT(*) INTO order_exists
    FROM order_details
    WHERE customer_ID = NEW.customer_ID AND product_id = NEW.product_id;
    
    IF order_exists = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You can only place a review for products you have ordered.';
    END IF;
END//


DELIMITER ;


DELIMITER //

CREATE TRIGGER delete_review_update_product_rating
AFTER DELETE ON reviews
FOR EACH ROW
BEGIN
    DECLARE avg_rating NUMERIC(4, 2);

    SELECT AVG(review_rating) INTO avg_rating
    FROM reviews
    WHERE product_id = OLD.product_id;

    IF avg_rating IS NULL THEN
        SET avg_rating = NULL;  
    END IF;

    UPDATE product
    SET product_rating = avg_rating
    WHERE product_id = OLD.product_id;
END //

DELIMITER ;


