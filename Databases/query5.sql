SELECT diet_plan.Breakfast AS Breakfast, diet_plan.Lunch AS Lunch, diet_plan.Dinner AS Dinner, diet_plan.Snack AS Snack
FROM blood_test JOIN diet_plan ON blood_test.PersonID = diet_plan.PersonID
WHERE blood_test.Cholesterol>250 AND diet_plan.Breakfast = 'Pancakes';