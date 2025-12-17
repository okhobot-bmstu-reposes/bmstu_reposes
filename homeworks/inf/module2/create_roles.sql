
BEGIN;

CREATE ROLE admin_role;
CREATE ROLE manager_role;
CREATE ROLE analyst_role;

GRANT ALL PRIVILEGES ON DATABASE postgres TO admin_role;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO admin_role;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO admin_role;

GRANT SELECT ON employees, projects TO manager_role;
GRANT INSERT, SELECT ON employee_projects TO manager_role;

GRANT SELECT ON projects, employee_projects TO analyst_role;

CREATE USER admin_user WITH PASSWORD 'admin123';
CREATE USER manager_user WITH PASSWORD 'manager123';
CREATE USER analyst_user WITH PASSWORD 'analyst123';

GRANT admin_role TO admin_user;
GRANT manager_role TO manager_user;
GRANT analyst_role TO analyst_user;

COMMIT;