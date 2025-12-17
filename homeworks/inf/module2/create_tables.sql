BEGIN;
CREATE TABLE employees (
    id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    position VARCHAR(100),
    salary DECIMAL(10, 2)
);

CREATE TABLE projects (
    id INT PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    budget DECIMAL(15, 2)
);

CREATE TABLE employee_projects (
    employee_id INT,
    project_id INT,
    role_in_project VARCHAR(100),
    PRIMARY KEY (employee_id, project_id),
    FOREIGN KEY (employee_id) REFERENCES employees(id) 
        ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (project_id) REFERENCES projects(id) 
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE audit_log (
    id SERIAL PRIMARY KEY,
    username VARCHAR(100),
    action VARCHAR(50),
    table_name VARCHAR(100),
    change_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    details TEXT
);
COMMIT;