## **📌 成绩管理系统设计**  

这个系统可以用于高校或培训机构，支持 **学生成绩管理、教师录入成绩、查询分析等功能**。  

---

## **📁 1. 数据库设计**  

使用 **MySQL** 设计数据库，包括 **学生、课程、成绩、教师** 等表。

### **📌 主要数据表**

#### **1️⃣ 学生表 (`students`)**
存储学生信息。  
```sql
CREATE TABLE students (
    student_id   INT PRIMARY KEY AUTO_INCREMENT,  -- 学号（主键）
    name         VARCHAR(50) NOT NULL,            -- 姓名
    gender       ENUM('男', '女') NOT NULL,       -- 性别
    birth_date   DATE,                            -- 生日
    class_name   VARCHAR(50) NOT NULL,           -- 班级
    phone        VARCHAR(20),                     -- 电话
    email        VARCHAR(100)                     -- 邮箱
);
```

#### **2️⃣ 课程表 (`courses`)**
存储课程信息。  
```sql
CREATE TABLE courses (
    course_id   INT PRIMARY KEY AUTO_INCREMENT,  -- 课程 ID
    name        VARCHAR(100) NOT NULL,           -- 课程名称
    credit      INT NOT NULL,                    -- 学分
    teacher_id  INT,                             -- 讲师 ID（外键）
    FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id)
);
```

#### **3️⃣ 教师表 (`teachers`)**
存储教师信息。  
```sql
CREATE TABLE teachers (
    teacher_id  INT PRIMARY KEY AUTO_INCREMENT,  -- 教师 ID
    name        VARCHAR(50) NOT NULL,            -- 姓名
    department  VARCHAR(100) NOT NULL,           -- 所属学院
    phone       VARCHAR(20),                     -- 电话
    email       VARCHAR(100)                     -- 邮箱
);
```

#### **4️⃣ 成绩表 (`grades`)**
存储学生成绩。  
```sql
CREATE TABLE grades (
    grade_id    INT PRIMARY KEY AUTO_INCREMENT,  -- 成绩 ID
    student_id  INT NOT NULL,                    -- 学生 ID（外键）
    course_id   INT NOT NULL,                    -- 课程 ID（外键）
    score       DECIMAL(5,2) NOT NULL,           -- 分数
    exam_date   DATE NOT NULL,                   -- 考试日期
    FOREIGN KEY (student_id) REFERENCES students(student_id),
    FOREIGN KEY (course_id) REFERENCES courses(course_id)
);
```

---

## **📝 2. 主要功能需求**

| **功能**         | **描述** |
|-----------------|---------|
| **学生管理**    | 增删改查学生信息 |
| **课程管理**    | 添加/编辑/删除课程 |
| **教师管理**    | 添加/编辑/删除教师信息 |
| **成绩录入**    | 录入学生成绩 |
| **成绩查询**    | 按 **学号、课程、教师** 查询成绩 |
| **成绩分析**    | 计算 **平均分、最高分、最低分、通过率** |
| **用户管理**    | 学生、教师、管理员角色权限控制 |

---

## **🎨 3. UI 设计**
UI 主要分为 **管理员、教师、学生界面**。  

### **📌 管理员界面**
1. **学生管理** 🧑‍🎓：
   - [✔] **增删改查** 学生信息
   - [✔] 按 **班级/姓名/学号** 搜索
2. **教师管理** 👨‍🏫：
   - [✔] **增删改查** 教师信息
   - [✔] 指定 **授课课程**
3. **课程管理** 📚：
   - [✔] 添加/修改/删除课程
   - [✔] 关联教师
4. **系统用户管理** 🔐：
   - [✔] 账号管理（分配**教师/学生/管理员**角色）

### **📌 教师界面**
1. **课程管理** 📖：
   - [✔] 查看自己教授的课程
2. **学生成绩管理** 🎓：
   - [✔] 录入/修改成绩
   - [✔] 批量导入成绩（Excel）
3. **成绩查询 & 统计** 📊：
   - [✔] **班级平均分、最高分、最低分、通过率**
   - [✔] 生成成绩报表（柱状图、折线图）

### **📌 学生界面**
1. **成绩查询** 🎯：
   - [✔] 按课程查询自己成绩
   - [✔] 查看班级成绩排名
2. **课程信息** 📜：
   - [✔] 查询课程列表和授课教师

---

## **💻 4. 可能的技术实现**
| **功能**  | **技术方案** |
|-----------|-------------|
| **前端**  | Qt Widgets / Qt Quick / Web 前端（Vue/React） |
| **后端**  | C++（Qt + MySQL） / Quarkus（Java） / Django（Python） |
| **数据库** | MySQL / PostgreSQL |
| **数据可视化** | Qt Charts / ECharts（前端） |
| **身份验证** | JWT / OAuth2 |

如果你希望用 **Qt + C++ 实现**，那么前端可以使用 **Qt Widgets** 来设计 GUI，后端直接用 Qt SQL 连接 MySQL，展示成绩数据。

你对这个设计有什么想法？需要具体的 Qt 代码示例吗？🚀