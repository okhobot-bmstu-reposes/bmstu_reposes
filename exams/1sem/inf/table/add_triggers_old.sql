-- Триггер для обновления даты заказа при изменении статуса
CREATE OR REPLACE FUNCTION update_order_date()
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
EXECUTE FUNCTION update_order_date();

-- Триггер для обновления общей суммы заказов при изменении цены товара
CREATE OR REPLACE FUNCTION update_order_totals_on_price_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.price != NEW.price THEN
        UPDATE orders o
        SET total_price = (
            SELECT SUM(oi.quantity * NEW.price)
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

-- Триггер для аудита изменений статусов заказов
CREATE OR REPLACE FUNCTION audit_order_status_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status != NEW.status THEN
        INSERT INTO order_status_history (order_id, old_status, new_status, changed_by)
        VALUES (NEW.order_id, OLD.status, NEW.status, NEW.user_id);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_order_status
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION audit_order_status_change();