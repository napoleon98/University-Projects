SELECT diet_plan.Breakfast AS Breakfast, diet_plan.Lunch AS Lunch, diet_plan.Dinner AS Dinner, diet_plan.Snack AS Snack, diet_plan.dietID AS dietID
FROM target JOIN person ON target.TargetID = person.TargetID
			JOIN diet_plan ON person.PersonID = diet_plan.PersonID
            WHERE target.Type = 'Increase';
                    
