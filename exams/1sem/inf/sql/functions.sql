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
CREATE OR REPLACE FUNCTION canReturnOrder(order_id INTEGER, user_id INTEGER)
RETURNS BOOLEAN AS $$
DECLARE
    order_status VARCHAR;
    order_date TIMESTAMP;
	order_user INTEGER;
BEGIN
    SELECT orders.status, orders.order_date, orders.user_id INTO order_status, order_date, order_user FROM orders WHERE orders.order_id = $1;
    RETURN order_status = 'completed' AND order_date >= (CURRENT_DATE - INTERVAL '30 days') AND order_user = user_id;
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


CREATE OR REPLACE FUNCTION generateCSVReport(p_admin_user_id INTEGER)
RETURNS TEXT AS $$
DECLARE
    v_csv_content TEXT;
    v_is_admin BOOLEAN;
BEGIN
    -- Проверка админа
    SELECT role = 'admin' INTO v_is_admin
    FROM users WHERE user_id = p_admin_user_id;

    IF NOT v_is_admin THEN
        RAISE EXCEPTION 'Доступ запрещен. Только администраторы могут генерировать отчеты.';
    END IF;

    -- Формируем CSV
    SELECT
        'user_id,user_name,user_role,total_orders,total_spent,status_changes,audit_actions' || E'\n' ||

        STRING_AGG(
            user_id::TEXT || ',' ||
            '"' || REPLACE(user_name, '"', '""') || '",' ||
            user_role || ',' ||
            total_orders::TEXT || ',' ||
            ROUND(total_spent, 2)::TEXT || ',' ||
            status_changes::TEXT || ',' ||
            audit_actions::TEXT,
            E'\n'
        ORDER BY total_spent DESC)

    INTO v_csv_content

    FROM (
        SELECT
            u.user_id,
            u.name as user_name,
            u.role as user_role,

            -- Заказы пользователя
            (SELECT COUNT(*) FROM orders o WHERE o.user_id = u.user_id) as total_orders,
            (SELECT COALESCE(SUM(total_price), 0) FROM orders o WHERE o.user_id = u.user_id) as total_spent,

            -- Изменения статусов (где пользователь был инициатором)
            (SELECT COUNT(*) FROM order_status_history osh WHERE osh.changed_by = u.user_id) as status_changes,

            -- Действия в аудите
            (SELECT COUNT(*) FROM audit_log al WHERE al.performed_by = u.user_id) as audit_actions

        FROM users u
    ) as report_data;


    RETURN v_csv_content;
END;
$$ LANGUAGE plpgsql;
