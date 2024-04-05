package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/15
 * @projectname schoolJavaHomework
 */
abstract class Employee {
    public abstract double earnings();
}

class YearWorker extends Employee {
    @Override
    public double earnings() {
        return 100000.0;
    }
}

class MonthWorker extends Employee {
    @Override
    public double earnings() {
        return 10000.0;
    }
}

class WeekWorker extends Employee {
    @Override
    public double earnings() {
        return 5000.0; // 每周领取0.5万，一个月4周
    }
}

class Company {
    Employee[] employees;
    double salaries = 0;

    Company(Employee[] employees) {
        this.employees = employees;
    }

    public double salariesPay() {
        salaries = 0;
        for (Employee employee : employees) {
            salaries += employee.earnings();
        }
        return salaries;
    }
}

public class HardWork {
    public static void main(String[] args) {
        Employee[] employees = new Employee[20];
        for (int i = 0; i < employees.length; i++) {
            if (i % 3 == 0)
                employees[i] = new WeekWorker();
            else if (i % 3 == 1)
                employees[i] = new MonthWorker();
            else if (i % 3 == 2)
                employees[i] = new YearWorker();
        }
        Company company = new Company(employees);
        System.out.println("公司年工资总额:" + company.salariesPay());
    }
}

