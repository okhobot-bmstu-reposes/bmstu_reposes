BEGIN;
    DELETE FROM employees WHERE id = 1; -- Ошибка: сотрудник есть в employee_projects
ROLLBACK;

BEGIN;
SET ROLE analyst_role;
    UPDATE projects SET budget = 10000 WHERE id = 1; -- Ошибка: нет прав
SET ROLE admin_role;
ROLLBACK;

BEGIN;
SET ROLE manager_role;
    UPDATE employees SET salary = 50000 WHERE id = 1; -- Ошибка: нет прав
SET ROLE admin_role;
ROLLBACK;