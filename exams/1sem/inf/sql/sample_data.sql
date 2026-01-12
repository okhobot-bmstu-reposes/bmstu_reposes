INSERT INTO users (name, email, role, password_hash, loyalty_level) VALUES
('Администратор', 'admin@store.com', 'admin', 'hashed_password_1', 1),
('Менеджер', 'manager@store.com', 'manager', 'hashed_password_2', 1),
('Покупатель', 'customer@store.com', 'customer', 'hashed_password_3', 0);

INSERT INTO products (name, price, stock_quantity) VALUES
('Ноутбук', 999.99, 10),
('Смартфон', 499.99, 25),
('Наушники', 99.99, 50);
CALL createOrder(
    3, -- user_id покупателя
    '[
        {"product_id": 1, "quantity": 1},
        {"product_id": 3, "quantity": 2}
    ]'::JSONB
);