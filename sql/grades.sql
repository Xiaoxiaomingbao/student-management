CREATE TABLE grades (
    grade_id    INTEGER PRIMARY KEY AUTOINCREMENT,   -- 成绩 ID
    student_id  INTEGER NOT NULL,                    -- 学生 ID
    course_id   INTEGER NOT NULL,                    -- 课程 ID
    score       DECIMAL(4,1) NOT NULL,               -- 分数
    FOREIGN KEY (student_id) REFERENCES students(student_id),
    FOREIGN KEY (course_id) REFERENCES courses(course_id)
);
