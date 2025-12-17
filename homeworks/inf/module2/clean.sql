BEGIN;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM admin_role, manager_role, analyst_role;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM admin_role, manager_role, analyst_role;
REVOKE ALL PRIVILEGES ON DATABASE postgres FROM admin_role, manager_role, analyst_role;

REVOKE admin_role FROM admin_user;
REVOKE manager_role FROM manager_user;
REVOKE analyst_role FROM analyst_user;

DROP TRIGGER IF EXISTS audit_employees_trigger ON employees;
DROP TRIGGER IF EXISTS audit_projects_trigger ON projects;
DROP TRIGGER IF EXISTS audit_employee_projects_trigger ON employee_projects;

DROP FUNCTION IF EXISTS audit_trigger_function CASCADE;

DROP TABLE IF EXISTS audit_log CASCADE;
DROP TABLE IF EXISTS employee_projects CASCADE;
DROP TABLE IF EXISTS projects CASCADE;
DROP TABLE IF EXISTS employees CASCADE;

DROP USER IF EXISTS admin_user;
DROP USER IF EXISTS manager_user;
DROP USER IF EXISTS analyst_user;

DROP ROLE IF EXISTS admin_role;
DROP ROLE IF EXISTS manager_role;
DROP ROLE IF EXISTS analyst_role;

COMMIT;