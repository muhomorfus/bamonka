import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '@/views/HomeView.vue'
import LoginView from "@/views/LoginView.vue";
import SearchView from "@/views/SearchView.vue";
import ShowEmployerView from "@/views/ShowEmployerView.vue";
import TeamsView from "@/views/TeamsView.vue";
import ShowTeamView from "@/views/ShowTeamView.vue";
import DepartmentsView from "@/views/DepartmentsView.vue";
import ShowDepartmentView from "@/views/ShowDepartmentView.vue";
import EmployeeView from "@/views/EmployeeView.vue";
import EditTeamView from "@/views/EditTeamView.vue";
import CreateTeamView from "@/views/CreateTeamView.vue";
import CreateDepartmentView from "@/views/CreateDepartmentView.vue";
import EditDepartmentView from "@/views/EditDepartmentView.vue";
import CreateEmployerView from "@/views/CreateEmployerView.vue";
import EditEmployerView from "@/views/EditEmployerView.vue";
import VacationsView from "@/views/VacationsView.vue";

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView
    },
    {
      path: '/login',
      name: 'login',
      component: LoginView
    },
    {
      path: '/search',
      name: 'search',
      component: SearchView
    },
    {
      path: '/employee/:nickname',
      name: 'view_employee',
      component: ShowEmployerView
    },
    {
      path: '/teams',
      name: 'teams',
      component: TeamsView
    },
    {
      path: '/teams/:id',
      name: 'view_team',
      component: ShowTeamView
    },
    {
      path: '/teams/:id/edit',
      name: 'edit_team',
      component: EditTeamView
    },
    {
      path: '/teams/create',
      name: 'create_team',
      component: CreateTeamView
    },
    {
      path: '/departments',
      name: 'departments',
      component: DepartmentsView
    },
    {
      path: '/departments/:id',
      name: 'view_department',
      component: ShowDepartmentView
    },
    {
      path: '/departments/:id/edit',
      name: 'edit_department',
      component: EditDepartmentView
    },
    {
      path: '/departments/create',
      name: 'create_department',
      component: CreateDepartmentView
    },
    {
      path: '/employee',
      name: 'employee',
      component: EmployeeView
    },
    {
      path: '/employee/create',
      name: 'create_employee',
      component: CreateEmployerView
    },
    {
      path: '/employee/:nickname/edit',
      name: 'edit_employee',
      component: EditEmployerView
    },
    {
      path: '/vacations',
      name: 'vacations',
      component: VacationsView
    },
  ]
})

export default router
