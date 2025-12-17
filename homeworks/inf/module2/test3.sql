BEGIN;
    UPDATE projects SET budget = 550000 WHERE id = 1;
COMMIT;

BEGIN;
    INSERT INTO employee_projects VALUES (2, 2, 'Разработчик');
COMMIT;

BEGIN;
    DELETE FROM employee_projects WHERE employee_id = 2 AND project_id = 2;
COMMIT;

BEGIN;
    INSERT INTO projects VALUES (3, 'Новый проект1', 750000);
    INSERT INTO employee_projects VALUES (1, 3, 'Тимлид');
COMMIT;
BEGIN;
    INSERT INTO projects VALUES (4, 'Новый проект2', 750000);
    INSERT INTO employee_projects VALUES (1, 4, 'Тимлид');
COMMIT;

BEGIN;
    DELETE FROM projects WHERE id = 4;
COMMIT;