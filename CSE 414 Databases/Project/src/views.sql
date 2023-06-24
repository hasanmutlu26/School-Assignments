CREATE VIEW order_details AS
SELECT po.orders_ID, po.customer_ID, op.product_id, op.product_quantity
FROM placed_order po
JOIN ordered_product op ON po.orders_ID = op.orders_ID;

CREATE VIEW product_order_view AS
SELECT op.orders_ID, p.product_name, op.product_quantity, p.product_price
FROM product p
JOIN ordered_product op ON p.product_id = op.product_id;

CREATE VIEW product_delivery_status AS
SELECT o.orders_ID, ds.delivery_status_ID, ds.delivery_station_name, ds.station_arrival_time
FROM orders o
JOIN order_delivery od ON o.orders_ID = od.orders_ID
JOIN delivery d ON od.delivery_ID = d.delivery_ID
JOIN delivery_status ds ON d.delivery_ID = ds.delivery_ID;
