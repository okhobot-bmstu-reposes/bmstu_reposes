
BEGIN;
    UPDATE projects SET budget = -5000 WHERE id = 1; -- Ошибка CHECK constraint
ROLLBACK;

BEGIN;
    INSERT INTO employee_projects VALUES (999, 1, 'Разработчик'); -- Ошибка FOREIGN KEY
ROLLBACK;