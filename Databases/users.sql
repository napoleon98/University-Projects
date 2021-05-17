#ADMIN
CREATE USER 'administrator'@'%' IDENTIFIED BY 'superpass';

GRANT ALL PRIVILEGES ON ourfitnessdb.* TO 'administrator'@'%';
#DIETITIAN
CREATE USER 'Dietitian'@'%' IDENTIFIED BY 'password';

GRANT SELECT, UPDATE, INSERT, DELETE ON ourfitnessdb.* TO 'Dietitian'@'%';
#SIMPLEUSER
CREATE USER 'simpleuser'@'%' IDENTIFIED BY 'simplepassword';

GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Person TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Person_has_Illness TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Blood_test TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Target TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Diet_plan TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Exercise_plan TO 'simpleuser'@'%';
GRANT SELECT,INSERT, UPDATE,DELETE ON ourfitnessdb.Recipe_belongs_to_diet_plan TO 'simpleuser'@'%';

GRANT SELECT ON ourfitnessdb.Food TO 'simpleuser'@'%';
GRANT SELECT ON ourfitnessdb.Recipe TO 'simpleuser'@'%';
GRANT SELECT ON ourfitnessdb.Recipe_contains_Food TO 'simpleuser'@'%';
GRANT SELECT ON ourfitnessdb.Illness TO 'simpleuser'@'%';
GRANT SELECT ON ourfitnessdb.Food_not_allowed_at_illness TO 'simpleuser'@'%';
#TRAINER
CREATE USER 'trainer'@'%'  IDENTIFIED BY 'password';

GRANT SELECT ON ourfitnessdb.* TO 'trainer'@'%';
GRANT INSERT, DELETE, UPDATE ON ourfitnessdb.exercise_plan TO 'trainer'@'%';