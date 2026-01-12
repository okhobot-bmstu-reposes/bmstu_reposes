-- Процедура создания заказа с транзакцией
CREATE OR REPLACE PROCEDURE createOrder(
    p_user_id INTEGER,
    p_items JSONB
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_order_id INTEGER;
    v_total DECIMAL(10,2) := 0;
    v_item JSONB;
    v_product_price DECIMAL(10,2);
    v_product_stock INTEGER;
BEGIN
    -- Проверяем существование пользователя
    IF NOT EXISTS (SELECT 1 FROM users WHERE user_id = p_user_id) THEN
        RAISE EXCEPTION 'Пользователь с ID % не найден', p_user_id;
    END IF;

    -- Начинаем транзакцию
    -- Вместо BEGIN/COMMIT используем управление через исключения
    -- Не используем отдельные BEGIN/COMMIT блоки!
    
    -- Создание заказа
    INSERT INTO orders (user_id, status) 
    VALUES (p_user_id, 'pending') 
    RETURNING order_id INTO v_order_id;
    
    -- Обработка каждого товара
    FOR v_item IN SELECT * FROM jsonb_array_elements(p_items)
    LOOP
        -- Получаем данные товара
        SELECT price, stock_quantity INTO v_product_price, v_product_stock
        FROM products 
        WHERE product_id = (v_item->>'product_id')::INTEGER
        FOR UPDATE;  -- Блокируем строку для обновления
        
        IF NOT FOUND THEN
            RAISE EXCEPTION 'Товар с ID % не найден', (v_item->>'product_id')::INTEGER;
        END IF;
        
        -- Проверка наличия товара
        IF v_product_stock < (v_item->>'quantity')::INTEGER THEN
            RAISE EXCEPTION 'Недостаточно товара на складе (ID товара: %, доступно: %, требуется: %)',
                (v_item->>'product_id')::INTEGER,
                v_product_stock,
                (v_item->>'quantity')::INTEGER;
        END IF;
        
        -- Добавление товара в заказ
        INSERT INTO order_items (order_id, product_id, quantity, price)
        VALUES (
            v_order_id,
            (v_item->>'product_id')::INTEGER,
            (v_item->>'quantity')::INTEGER,
            v_product_price
        );
        
        -- Обновление суммы заказа
        v_total := v_total + (v_product_price * (v_item->>'quantity')::INTEGER);
        
        -- Уменьшение количества товара на складе
        UPDATE products 
        SET stock_quantity = stock_quantity - (v_item->>'quantity')::INTEGER
        WHERE product_id = (v_item->>'product_id')::INTEGER;
    END LOOP;
    
    -- Обновление итоговой суммы заказа
    UPDATE orders SET total_price = v_total WHERE order_id = v_order_id;
    
    -- Логирование в аудит (автоматически через триггер)
    -- INSERT в audit_log сделает триггер
    
    -- Не вызываем COMMIT явно - процедура сама в транзакции
    -- COMMIT происходит автоматически при успешном завершении
    
EXCEPTION WHEN OTHERS THEN
    -- При ошибке произойдёт автоматический ROLLBACK
    RAISE;
END;
$$;

-- Процедура обновления статуса заказа
CREATE OR REPLACE PROCEDURE updateOrderStatus(
    p_order_id INTEGER,
    p_new_status VARCHAR,
    p_changed_by_user_id INTEGER  -- Кто реально меняет статус
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_old_status VARCHAR;
BEGIN
    -- Получение текущего статуса
    SELECT status INTO v_old_status 
    FROM orders 
    WHERE order_id = p_order_id;
    
    -- Обновление статуса
    UPDATE orders 
    SET status = p_new_status 
    WHERE order_id = p_order_id;
    
    
    -- Используем временную таблицу/параметр сессии
    PERFORM set_config('app.status_changer', p_changed_by_user_id::TEXT, FALSE);
    
END;
$$;