-- Удаление таблиц в правильном порядке (сначала зависимые, потом родительские)
DROP TABLE IF EXISTS audit_log CASCADE;
DROP TABLE IF EXISTS order_status_history CASCADE;
DROP TABLE IF EXISTS order_items CASCADE;
DROP TABLE IF EXISTS orders CASCADE;
DROP TABLE IF EXISTS products CASCADE;
DROP TABLE IF EXISTS users CASCADE;

-- Создание таблицы пользователей
CREATE TABLE users (
    user_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    name VARCHAR NOT NULL,
    email VARCHAR UNIQUE NOT NULL,
    role VARCHAR CHECK (role IN ('admin', 'manager', 'customer')),
    password_hash VARCHAR NOT NULL,
    loyalty_level INTEGER CHECK (loyalty_level IN (0, 1))
);

-- Создание таблицы продуктов
CREATE TABLE products (
    product_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    name VARCHAR NOT NULL,
    price DECIMAL CHECK (price > 0),
    stock_quantity INTEGER CHECK (stock_quantity >= 0)
);

-- Создание таблицы заказов
CREATE TABLE orders (
    order_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    user_id INTEGER REFERENCES users(user_id),
    status VARCHAR CHECK (status IN ('pending', 'completed', 'canceled', 'returned')),
    total_price DECIMAL,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Создание таблицы элементов заказов
CREATE TABLE order_items (
    order_item_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    order_id INTEGER REFERENCES orders(order_id),
    product_id INTEGER REFERENCES products(product_id),
    quantity INTEGER CHECK (quantity > 0),
    price DECIMAL
);

-- Создание таблицы истории статусов заказов
CREATE TABLE order_status_history (
    history_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    order_id INTEGER REFERENCES orders(order_id),
    old_status VARCHAR,
    new_status VARCHAR,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER REFERENCES users(user_id)
);

-- Создание таблицы логирования операций
CREATE TABLE audit_log (
    log_id INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    entity_type VARCHAR CHECK (entity_type IN ('order', 'product', 'user')),
    entity_id INTEGER,
    operation VARCHAR CHECK (operation IN ('insert', 'update', 'delete')),
    performed_by INTEGER REFERENCES users(user_id),
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);