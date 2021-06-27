/**
服装销售系统的设计与实现
基本要求：包含三类用户：管理员、店长、销售员；
（1）管理员功能：自身密码修改；其他用户的添加、删除；用户信息的修改、统计；商品信息的添加、修改、删除、查找、统计。
（2）店长功能：登录、注销、自身密码修改、自身信息修改；商品信息的修改、统计；查看日报表、月报表、商品销售量报表、营业员业绩报表；查找、浏览、修改商品储备信息。
（3）销售员功能：商品浏览、查找、出售商品，以及查看自己本日报表、本月报表。
（4）界面友好，演示程序以用户和计算机的对话方式进行，可反复操作。
提高部分：利用外部.txt文件同步记录所有数据。
 */

#include <stdio.h>

#include <string.h>

typedef struct MANAGER Manager;
typedef struct PRODUCT Product;
typedef struct PERSON Person;


struct PRODUCT {
    char no[20];
    char name[20];
    int inventory; // 库存
    double price; // 价格
    int sale; // 销售量
};


struct PERSON {
    char no[20];
    char password[20];
    char name[20];
    double sale[32];
    double tot; // 月销售额
};

struct MANAGER {
    char no[20];
    char password[20];
    char name[20];
    double tot; // 月销售额
};


Manager managers[100];
int managers_n = 0; // 管理者数量
int current_manager;
int current_person; //从属于current_manager
Product products[100];
int products_n;
char admin_password[20] = "admin";

Person persons[20]; // 所有销售
int persons_n;// 有几个销售

void save() {
    FILE *f = fopen("data.txt", "w");
    int i, j;
    fprintf(f, "%s\n", admin_password);
    fprintf(f, "%d\n", managers_n);
    for (i = 0; i < managers_n; i++) {
        fprintf(f, "%s %s %s %lf\n", managers[i].no, managers[i].password, managers[i].name, managers[i].tot);
    }
    fprintf(f, "%d\n", products_n);
    for (i = 0; i < products_n; i++) {
        fprintf(f, "%s %s %d %lf %d\n", products[i].no, products[i].name, products[i].inventory, products[i].price,
                products[i].sale);
    }
    fprintf(f, "%d\n", persons_n);
    for (i = 0; i < persons_n; i++) {
        fprintf(f, "%s %s %s %lf", persons[i].no, persons[i].password, persons[i].name, persons[i].tot);
        for (j = 0; j < 32; j++)fprintf(f, " %lf", persons[i].sale[j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void read() {
    int i, j;
    FILE *f = fopen("data.txt", "r");
    if (f == NULL)return;
    fscanf(f, "%s", admin_password);
    fscanf(f, "%d", &managers_n);
    for (i = 0; i < managers_n; i++) {
        fscanf(f, "%s %s %s %lf", managers[i].no, managers[i].password, managers[i].name, &managers[i].tot);
    }
    fscanf(f, "%d", &products_n);
    for (i = 0; i < products_n; i++) {
        fscanf(f, "%s %s %d %lf %d", products[i].no, products[i].name, &products[i].inventory, &products[i].price,
               &products[i].sale);
    }
    fscanf(f, "%d", &persons_n);
    for (i = 0; i < persons_n; i++) {
        fscanf(f, "%s %s %s %lf", persons[i].no, persons[i].password, persons[i].name, &persons[i].tot);
        for (j = 0; j < 32; j++)fscanf(f, "%lf", &persons[i].sale[j]);
    }
    fclose(f);
}

void print_person_header() {
    printf("编号\t姓名\t月销售额\n");
}

void print_a_person(Person p) {
    printf("%s\t%s\t%lf\n", p.no, p.name, p.tot);
}

void print_all_persons() {
    int i, choose, j;
    Person tmp_persons[20]; // 临时数据
    Person t;

    printf("(1)普通排列\n");
    printf("(2)月销售额降序排列\n");
    scanf("%d", &choose);

    print_person_header();
    if (choose == 1) {
        for (i = 0; i < persons_n; i++)
            print_a_person(persons[i]);
    } else {
        for (i = 0; i < persons_n; i++) {//降序输出
            tmp_persons[i] = persons[i];
        }
        for (i = 0; i < persons_n; i++) {
            for (j = 0; j < persons_n - 1; j++) {
                if (tmp_persons[j].tot < tmp_persons[j + 1].tot) {
                    t = tmp_persons[j];
                    tmp_persons[j] = tmp_persons[j + 1];
                    tmp_persons[j + 1] = t;
                }
            }
        }


        for (i = 0; i < persons_n; i++)
            print_a_person(tmp_persons[i]);
    }

}

void print_product_header() {
    printf("编号\t名称\t库存\t已售\t价格\n");
}

void print_a_product(Product p) {
    printf("%s\t%s\t%d\t%d\t%lf\n", p.no, p.name, p.inventory, p.sale, p.price);
}

void print_all_products() {
    int i;
    print_product_header();
    for (i = 0; i < products_n; i++)
        print_a_product(products[i]);
}

int search_product(char *no) {
    // 通过编号查找商品
    int i;
    for (i = 0; i < products_n; i++)
        if (strcmp(products[i].no, no) == 0)
            return i;
    return -1; // 等于-1就是搜索不到这个商品
}

int search_manager(char *no) {
    int i;
    for (i = 0; i < managers_n; i++)
        if (strcmp(managers[i].no, no) == 0) //strcmp==0意思是两个值相同
            return i;
    return -1;
}

int search_person(char *no) {
    int i;
    for (i = 0; i < persons_n; i++)
        if (strcmp(persons[i].no, no) == 0) //strcmp==0意思是两个值相同
            return i;
    return -1;
}

void insert_product() {
    Product p;
    int index;
    printf("请输入商品编号：");
    scanf("%s", p.no);
    index = search_product(p.no);
    if (index != -1) {
        printf("此编号已存在！\n");
        return;
    }
    printf("请输入商品名称：");
    scanf("%s", p.name);
    printf("请输入商品库存：");
    scanf("%d", &p.inventory);
    printf("请输入商品价格：");
    scanf("%lf", &p.price);
    p.sale=0;
    products[products_n++] = p;
    printf("添加成功！\n");
}

void delete_product() {
    char no[20];
    int j;
    printf("请输入您想删除的商品的编号：");
    scanf("%s", no);

    int index = search_product(no);
    if (index == -1) {
        printf("您要查找的编号不存在！\n");
        return;
    } else {
        products_n--;
        for (j = index; j < products_n; j++)
            products[j] = products[j + 1];
        printf("删除成功！\n");
    }
}

void print_manager_header() {
    printf("编号\t姓名\t月销售额\n");
}

void print_a_manager(Manager m) {
    printf("%s\t%s\t%lf\n", m.no, m.name, m.tot);
}

void print_all_managers() {
    int i;
    print_manager_header();
    for (i = 0; i < managers_n; i++)
        print_a_manager(managers[i]);
}

void insert_manager() {
    Manager m;
    int index;
    printf("请输入商家编号：");
    scanf("%s", m.no);
    index = search_manager(m.no);
    if (index != -1) {
        printf("此编号已存在！\n");
        return;
    }

    printf("请输入用户姓名：");
    scanf("%s", m.name);
    printf("请输入用户密码：");
    scanf("%s", m.password);

    m.tot = 0;
    managers[managers_n++] = m;

    printf("添加成功！\n");
}

void delete_manager() {
    char no[20];
    int j;
    print_all_managers();
    printf("请输入您想删除的店主的编号：");
    scanf("%s", no);

    int index = search_manager(no);
    if (index == -1) {
        printf("您要查找的编号不存在！\n");
        return;
    } else {
        managers_n--;
        for (j = index; j < managers_n; j++)
            managers[j] = managers[j + 1];
        printf("删除成功！\n");
    }
}

void edit_manager() {
    int choose;

    char manager_no[20];
    print_all_managers();
    printf("请输入您要修改的店主的编号：");
    scanf("%s", manager_no);
    int manager_index = search_manager(manager_no);
    if (manager_index == -1) {
        printf("没有此编号的销售！请重试！\n");
        return;
    }


    printf("当前店主信息：\n");
    print_manager_header();
    print_a_manager(managers[manager_index]);

    printf("(1)用户名\n");
    printf("(2)密码\n");
    printf("(3)姓名\n");
    printf("(4)退出\n");
    printf("请输入您要修改什么信息：");
    scanf("%d", &choose);
    if (choose == 1) {
        printf("请输入新的用户名：");
        scanf("%s", managers[manager_index].no);
    } else if (choose == 2) {
        printf("请输入新的密码：");
        scanf("%s", managers[manager_index].password);
    } else if (choose == 3) {
        printf("请输入新的姓名：");
        scanf("%s", managers[manager_index].name);
    } else if (choose == 4) {
        return;
    }
    printf("修改成功！\n");
}

void edit_product() {
    int choose;

    char product_no[20];
    print_all_products();
    printf("请输入您要修改的商品的编号：");
    scanf("%s", product_no);
    int product_index = search_product(product_no);
    if (product_index == -1) {
        printf("没有此编号的商品！请重试！\n");
        return;
    }

    printf("当前商品信息：\n");
    print_product_header();
    print_a_product(products[product_index]);

    printf("(1)编号\n");
    printf("(2)名称\n");
    printf("(3)库存\n");
    printf("(4)价格\n");
    printf("(5)退出\n");
    printf("请输入您要修改什么信息：");
    scanf("%d", &choose);
    if (choose == 1) {
        printf("请输入新的编号：");
        scanf("%s", products[product_index].no);
    } else if (choose == 2) {
        printf("请输入新的名称：");
        scanf("%s", products[product_index].name);
    } else if (choose == 3) {
        printf("请输入新的库存：");
        scanf("%d", &products[product_index].inventory);
    } else if (choose == 4) {
        printf("请输入新的价格：");
        scanf("%lf", &products[product_index].price);
    } else if (choose == 4) {
        return;
    }
    printf("修改成功！\n");
}

void manager_menu() {
    int choose;
    while (1) {
        printf("\n\n\n");
        printf("1.修改商品信息\n");
        printf("2.查看所有商品\n");
        printf("3.查看月报表\n");
        printf("4.退出\n");
        scanf("%d", &choose);
        if (choose == 1) {
            edit_product();
        } else if (choose == 2) {
            print_all_products();
        } else if (choose == 3) {
            print_manager_header();
            print_a_manager(managers[current_manager]);
        } else if (choose == 4) {
            break;
        }
        save();
    }
}

void insert_person() {
    Person p;
    int index, i;
    printf("请输入销售编号：");
    scanf("%s", p.no);
    index = search_person(p.no);
    if (index != -1) {
        printf("此编号已存在！\n");
        return;
    }

    printf("请输入销售姓名：");
    scanf("%s", p.name);
    printf("请输入销售密码：");
    scanf("%s", p.password);

    for (i = 0; i < 32; i++)
        p.sale[i] = 0;
    p.tot = 0;
    persons[persons_n++] = p;

    printf("添加成功！\n");
}

void edit_person() {
    int choose, i;

    char person_no[20];

    print_person_header();
    for (i = 0; i < persons_n; i++)
        print_a_person(persons[i]);
    printf("请输入您要修改的销售的编号：");
    scanf("%s", person_no);
    int person_index = search_person(person_no);
    if (person_index == -1) {
        printf("没有此编号的销售！请重试！\n");
        return;
    }


    printf("当前销售信息：\n");
    print_person_header();
    print_a_person(persons[person_index]);

    printf("(1)用户名\n");
    printf("(2)密码\n");
    printf("(3)姓名\n");
    printf("(4)退出\n");
    printf("请输入您要修改什么信息：");
    scanf("%d", &choose);
    if (choose == 1) {
        printf("请输入新的用户名：");
        scanf("%s", persons[person_index].no);
    } else if (choose == 2) {
        printf("请输入新的密码：");
        scanf("%s", persons[person_index].password);
    } else if (choose == 3) {
        printf("请输入新的姓名：");
        scanf("%s", persons[person_index].name);
    } else if (choose == 4) {
        return;
    }
    printf("修改成功！\n");
}

void delete_person() {
    char no[20];
    int j;
    printf("请输入您想删除的销售的编号：");
    scanf("%s", no);

    int index = search_person(no);
    if (index == -1) {
        printf("您要查找的编号不存在！\n");
        return;
    } else {
        persons_n--;
        for (j = index; j < persons_n; j++)
            persons[j] = persons[j + 1];
        printf("删除成功！\n");
    }
}

void sale_product() {
    if (managers_n == 0) {
        printf("您的系统中还没有商家！请添加后重试！\n");
        return;
    }
    if (products_n == 0) {
        printf("您的系统中还没有商品！请添加后重试！\n");
        return;
    }

    char product_no[20];
    print_all_products();
    printf("请输入您已经出售的商品的编号：");
    scanf("%s", product_no);
    int product_index = search_product(product_no);
    if (product_index == -1) {
        printf("没有此编号的商品！请重试！\n");
        return;
    }

    Product product = products[product_index];

    char manager_no[20];
    print_all_managers();
    printf("请输入您出售的商品来源商家的编号：");
    scanf("%s", manager_no);
    int manager_index = search_manager(manager_no);
    if (manager_index == -1) {
        printf("没有此编号的商家！请重试！\n");
        return;
    }


    int num;
    printf("请输入您已经出售的商品的数量：");
    scanf("%d", &num);

    if (product.inventory < num) {
        printf("商品库存不足！\n");
        return;
    }

    int day;
    printf("请输入您几号出售的（1～31）：");
    scanf("%d", &day);

    persons[current_person].sale[day] += product.price * num;
    persons[current_person].tot += product.price * num;
    products[product_index].inventory -= num;
    managers[manager_index].tot += product.price * num;

    printf("出售成功！\n");
}

void admin_menu() {
    //（1）管理员功能：自身密码修改；其他用户的添加、删除；用户信息的修改、统计；商品信息的添加、修改、删除、查找、统计。
    int choose, i;
    while (1) {
        printf("\n\n\n");
        printf("1.修改密码\n");
        printf("2.查看所有店主\n");
        printf("3.添加店主\n");
        printf("4.修改店主\n");
        printf("5.删除店主\n");
        printf("6.查看所有商品\n");
        printf("7.添加商品\n");
        printf("8.修改商品\n");
        printf("9.删除商品\n");
        printf("10.查看所有销售员\n");
        printf("11.添加销售员\n");
        printf("12.修改销售员\n");
        printf("13.删除销售员\n");
        printf("14.退出\n");
        scanf("%d", &choose);
        if (choose == 1) {
            printf("请输入新密码：");
            scanf("%s", admin_password);
        } else if (choose == 2) {
            print_all_managers();
        } else if (choose == 3) {
            insert_manager();
        } else if (choose == 4) {
            edit_manager();
        } else if (choose == 5) {
            delete_manager();
        } else if (choose == 6) {
            print_all_products();
        } else if (choose == 7) {
            insert_product();
        } else if (choose == 8) {
            edit_product();
        } else if (choose == 9) {
            delete_product();
        } else if (choose == 10) {
            print_all_persons();
        } else if (choose == 11) {
            insert_person();
        } else if (choose == 12) {
            edit_person();
        } else if (choose == 13) {
            delete_person();
        } else if (choose == 14) {
            break;
        }
    }
    save();
}

void person_menu() {

    int choose, i;
    while (1) {
        printf("\n\n\n");
        printf("1.查看所有商品\n");
        printf("2.出售商品\n");
        printf("3.查看日报表\n");
        printf("4.查看月报表\n");
        printf("5.退出\n");
        scanf("%d", &choose);
        if (choose == 1) {
            print_all_products();
        } else if (choose == 2) {
            sale_product();
        } else if (choose == 3) {
            for (i = 1; i <= 31; i++) {
                if (persons[current_person].sale[i] != 0) {
                    printf("第%d日销售：%lf 元\n", i, persons[current_person].sale[i]);
                }
            }
        } else if (choose == 4) {
            printf("月销售额：%lf\n", persons[current_person].tot);
        } else if (choose == 5) {
            break;
        }
        save();
    }
}

int main() {
    char no[20];
    char password[20];
    int choose, i, j;

    managers_n = 0;
    persons_n = 0;
    products_n = 0;
    printf("欢迎登陆商品交易系统!\n");
    read();
    while (1) {
        printf("1.管理员\n");
        printf("2.店长\n");
        printf("3.销售\n");
        printf("4.退出\n");
        printf("请选择您的身份：");
        scanf("%d", &choose);
        if (choose == 1) {
            printf("请输入您的用户名:");
            scanf("%s", no);
            printf("请输入您的密码:");
            scanf("%s", password);
            if (strcmp("admin", no) == 0 && strcmp(admin_password, password) == 0) {
                admin_menu();
            }

        }
        if (choose == 2) {
            printf("请输入您的用户名:");
            scanf("%s", no);
            printf("请输入您的密码:");
            scanf("%s", password);
            for (i = 0; i < managers_n; i++) {
                if (strcmp(managers[i].no, no) == 0 && strcmp(managers[i].password, password) == 0) {
                    current_manager = i;
                    manager_menu();
                    break;
                }
            }
        }
        if (choose == 3) {
            printf("请输入您的用户名:");
            scanf("%s", no);
            printf("请输入您的密码:");
            scanf("%s", password);
            for (i = 0; i < persons_n; i++) {
                if (strcmp(persons[i].no, no) == 0 && strcmp(persons[i].password, password) == 0) {
                    current_person = i;
                    person_menu();
                    break;
                }
            }
        }
        if (choose == 4)
            break;
        save();
    }
}