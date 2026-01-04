-- 1. Триггер для аудита изменений статусов заказов (автоматически записывается в order_status_history)
CREATE OR REPLACE FUNCTION audit_order_status_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status != NEW.status THEN
        -- Запись в историю статусов
        INSERT INTO order_status_history (order_id, old_status, new_status, changed_by)
        VALUES (NEW.order_id, OLD.status, NEW.status, NEW.user_id);
        
        -- Аудит изменения статуса заказа
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('order', NEW.order_id, 'update', NEW.user_id);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_order_status
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION audit_order_status_change();

-- 2. Триггер для аудита создания, отмены, возврата заказа (вставка и удаление заказов)
CREATE OR REPLACE FUNCTION audit_orders()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        -- Аудит создания заказа
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('order', NEW.order_id, 'insert', NEW.user_id);
        RETURN NEW;
        
    ELSIF TG_OP = 'DELETE' THEN
        -- Аудит удаления заказа
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('order', OLD.order_id, 'delete', OLD.user_id);
        RETURN OLD;
        
    ELSIF TG_OP = 'UPDATE' THEN
        -- Аудит других изменений заказа (кроме статуса, который уже обработан выше)
        IF OLD.status != NEW.status AND TG_NARGS = 0 THEN
            -- Проверяем специальные статусы: отмена и возврат
            IF NEW.status = 'canceled' THEN
                INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
                VALUES ('order', NEW.order_id, 'update', NEW.user_id);
            ELSIF NEW.status = 'returned' THEN
                INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
                VALUES ('order', NEW.order_id, 'update', NEW.user_id);
            END IF;
        END IF;
        RETURN NEW;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_orders
AFTER INSERT OR UPDATE OR DELETE ON orders
FOR EACH ROW
EXECUTE FUNCTION audit_orders();

-- 3. Триггер для аудита операций с товарами (добавление, обновление, удаление)
CREATE OR REPLACE FUNCTION audit_products()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        -- Аудит добавления товара
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('product', NEW.product_id, 'insert', 
                (SELECT user_id FROM users LIMIT 1)); -- Админ по умолчанию
        RETURN NEW;
        
    ELSIF TG_OP = 'UPDATE' THEN
        -- Аудит обновления товара (включая изменение цены)
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('product', NEW.product_id, 'update',
                (SELECT user_id FROM users LIMIT 1));
        RETURN NEW;
        
    ELSIF TG_OP = 'DELETE' THEN
        -- Аудит удаления товара
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('product', OLD.product_id, 'delete',
                (SELECT user_id FROM users LIMIT 1));
        RETURN OLD;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_products
AFTER INSERT OR UPDATE OR DELETE ON products
FOR EACH ROW
EXECUTE FUNCTION audit_products();

-- 4. Триггер для аудита операций с пользователями
CREATE OR REPLACE FUNCTION audit_users()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        -- Аудит создания пользователя (сам себя)
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('user', NEW.user_id, 'insert', NEW.user_id);
        RETURN NEW;
        
    ELSIF TG_OP = 'UPDATE' THEN
        -- Аудит обновления пользователя
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('user', NEW.user_id, 'update', NEW.user_id);
        RETURN NEW;
        
    ELSIF TG_OP = 'DELETE' THEN
        -- Аудит удаления пользователя (нужен отдельный механизм для определения кто удалил)
        -- Используем текущего пользователя из сессии или системного админа
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('user', OLD.user_id, 'delete',
                (SELECT user_id FROM users WHERE role = 'admin' LIMIT 1));
        RETURN OLD;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_users
AFTER INSERT OR UPDATE OR DELETE ON users
FOR EACH ROW
EXECUTE FUNCTION audit_users();

-- 5. Триггер для обновления даты заказа при изменении статуса
CREATE OR REPLACE FUNCTION update_order_date_on_status_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status != NEW.status THEN
        NEW.order_date = CURRENT_TIMESTAMP;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_update_order_date
BEFORE UPDATE ON orders
FOR EACH ROW
EXECUTE FUNCTION update_order_date_on_status_change();

-- 6. Триггер для обновления общей суммы заказов при изменении цены товара
CREATE OR REPLACE FUNCTION update_order_totals_on_price_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.price != NEW.price THEN
        UPDATE orders o
        SET total_price = (
            SELECT COALESCE(SUM(oi.quantity * NEW.price), 0)
            FROM order_items oi
            WHERE oi.order_id = o.order_id
            AND oi.product_id = NEW.product_id
        )
        WHERE order_id IN (
            SELECT DISTINCT order_id 
            FROM order_items 
            WHERE product_id = NEW.product_id
        );
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_update_order_totals
AFTER UPDATE OF price ON products
FOR EACH ROW
EXECUTE FUNCTION update_order_totals_on_price_change();

-- 7. Дополнительный триггер для аудита элементов заказа
CREATE OR REPLACE FUNCTION audit_order_items()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        -- Аудит добавления товара в заказ
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('order', NEW.order_id, 'update',
                (SELECT user_id FROM orders WHERE order_id = NEW.order_id));
        RETURN NEW;
        
    ELSIF TG_OP = 'DELETE' THEN
        -- Аудит удаления товара из заказа
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
        VALUES ('order', OLD.order_id, 'update',
                (SELECT user_id FROM orders WHERE order_id = OLD.order_id));
        RETURN OLD;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_order_items
AFTER INSERT OR DELETE ON order_items
FOR EACH ROW
EXECUTE FUNCTION audit_order_items();