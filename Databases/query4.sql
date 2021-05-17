SELECT person.PersonID as PersonID, exercise_plan.exID as ExerciseID, exercise_plan.Type 
FROM person JOIN exercise_plan ON person.exID = exercise_plan.exID
WHERE person.Weight > 100 AND person.age > 50;
