import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadDepartment(ctx: any, id: string) {
            try {
                const resp = await API.getDepartment(id)
                ctx.commit('saveDepartment', resp.data)

                const bc = await API.getDepartmentBreadcrumbs(id);
                ctx.commit('saveBreadcrumbs', bc.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async loadDepartmentMembers(ctx: any, id: string) {
            try {
                const resp = await API.getEmployeeByDepartment(id)
                ctx.commit('saveDepartmentMembers', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async createDepartment(ctx: any) {
            try {
                await API.createDepartment({
                    name: ctx.state.department.name,
                    description: ctx.state.department.description,
                    parent_id: ctx.state.department.parent_id,
                })
                ctx.dispatch('showError', 'Department successfully created.')
                window.location.replace('/departments')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async saveDepartment(ctx: any) {
            try {
                await API.updateDepartment(ctx.state.department.id, {
                    name: ctx.state.department.name,
                    description: ctx.state.department.description,
                    parent_id: ctx.state.department.parent_id,
                })
                ctx.dispatch('showError', 'Department successfully saved.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },
    },

    mutations: {
        saveDepartment(state: any, department: openapi.Department) {
            state.department = department
        },

        saveDepartmentMembers(state: any, emp: Array<openapi.Employee>) {
            state.departmentMembers = emp
        },

        saveBreadcrumbs(state: any, breadcrumbs: Array<openapi.DepartmentShort>) {
            state.breadcrumbs = breadcrumbs.reverse()
        },
    },

    state: {
        department: {} as openapi.Department,
        departmentMembers: Array<openapi.Employee>(),
        breadcrumbs: Array<openapi.DepartmentShort>(),
    },

    getters: {
        department(state: any) {
            return state.department
        },

        departmentMembers(state: any) {
            return state.departmentMembers
        },

        breadcrumbs(state: any) {
            return state.breadcrumbs
        },
    },
};