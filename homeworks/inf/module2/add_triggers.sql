BEGIN;

CREATE OR REPLACE FUNCTION audit_trigger_function()
RETURNS TRIGGER AS $$
DECLARE
    v_record_info TEXT;
BEGIN

    IF TG_OP = 'INSERT' THEN
        v_record_info := 'Добавлена запись: ' || row_to_json(NEW)::TEXT;
    ELSIF TG_OP = 'UPDATE' THEN
        v_record_info := 'Изменено: ' || row_to_json(OLD)::TEXT || ' -> ' || row_to_json(NEW)::TEXT;
    ELSIF TG_OP = 'DELETE' THEN
        v_record_info := 'Удалена запись: ' || row_to_json(OLD)::TEXT;
    END IF;

    INSERT INTO audit_log (username, action, table_name, details)
    VALUES (
        current_user, 
        TG_OP, 
        TG_TABLE_NAME, 
        v_record_info
    );
    
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER audit_employees_trigger
AFTER INSERT OR UPDATE OR DELETE ON employees
FOR EACH ROW EXECUTE FUNCTION audit_trigger_function();

CREATE TRIGGER audit_projects_trigger
AFTER INSERT OR UPDATE OR DELETE ON projects
FOR EACH ROW EXECUTE FUNCTION audit_trigger_function();

CREATE TRIGGER audit_employee_projects_trigger
AFTER INSERT OR UPDATE OR DELETE ON employee_projects
FOR EACH ROW EXECUTE FUNCTION audit_trigger_function();

COMMIT;