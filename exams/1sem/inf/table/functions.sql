-- Функция для получения статуса заказа
CREATE OR REPLACE FUNCTION getOrderStatus(order_id INTEGER)
RETURNS VARCHAR AS $$
    SELECT status FROM orders WHERE order_id = $1;
$$ LANGUAGE SQL;

-- Функция для получения количества заказов пользователя
CREATE OR REPLACE FUNCTION getUserOrderCount(user_id INTEGER)
RETURNS INTEGER AS $$
    SELECT COUNT(*) FROM orders WHERE user_id = $1;
$$ LANGUAGE SQL;

-- Функция для получения общей суммы покупок пользователя
CREATE OR REPLACE FUNCTION getTotalSpentByUser(user_id INTEGER)
RETURNS DECIMAL AS $$
    SELECT COALESCE(SUM(total_price), 0) FROM orders WHERE user_id = $1;
$$ LANGUAGE SQL;

-- Функция для проверки возможности возврата заказа
CREATE OR REPLACE FUNCTION canReturnOrder(order_id INTEGER)
RETURNS BOOLEAN AS $$
DECLARE
    order_status VARCHAR;
    order_date TIMESTAMP;
BEGIN
    SELECT status, order_date INTO order_status, order_date FROM orders WHERE order_id = $1;
    RETURN order_status = 'completed' AND order_date >= (CURRENT_DATE - INTERVAL '30 days');
END;
$$ LANGUAGE plpgsql;

-- Функция для получения истории статусов заказа
CREATE OR REPLACE FUNCTION getOrderStatusHistory(order_id INTEGER)
RETURNS TABLE(
    old_status VARCHAR,
    new_status VARCHAR,
    changed_at TIMESTAMP,
    changed_by_name VARCHAR
) AS $$
    SELECT 
        osh.old_status,
        osh.new_status,
        osh.changed_at,
        u.name as changed_by_name
    FROM order_status_history osh
    JOIN users u ON osh.changed_by = u.user_id
    WHERE osh.order_id = $1
    ORDER BY osh.changed_at;
$$ LANGUAGE SQL;

-- Функция для получения логов аудита пользователя
CREATE OR REPLACE FUNCTION getAuditLogByUser(user_id INTEGER)
RETURNS TABLE(
    entity_type VARCHAR,
    entity_id INTEGER,
    operation VARCHAR,
    performed_at TIMESTAMP
) AS $$
    SELECT 
        entity_type,
        entity_id,
        operation,
        performed_at
    FROM audit_log
    WHERE performed_by = $1
    ORDER BY performed_at DESC;
$$ LANGUAGE SQL;