BEGIN;
INSERT INTO employees (id, name, position, salary) VALUES
(1, 'Иван Иванов', 'Разработчик', 80000),
(2, 'Петр Петров', 'Менеджер', 120000),
(3, 'Анна Сидорова', 'Аналитик', 75000);

INSERT INTO projects (id, name, budget) VALUES
(1, 'Проект А', 500000),
(2, 'Проект Б', 300000),
(3, 'Проект В', 750000);

INSERT INTO employee_projects (employee_id, project_id, role_in_project) VALUES
(1, 1, 'Ведущий разработчик'),
(2, 1, 'Менеджер проекта'),
(3, 2, 'Бизнес-аналитик'),
(1, 3, 'Архитектор');

COMMIT;