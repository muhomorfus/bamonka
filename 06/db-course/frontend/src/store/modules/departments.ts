import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadDepartments(ctx: any) {
            try {
                const resp = await API.getDepartmentList()
                ctx.commit('saveDepartments', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async deleteDepartment(ctx: any, id: string) {
            try {
                await API.deleteDepartment(id)
                ctx.commit('removeDepartment', id)
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
        saveDepartments(state: any, departments: Array<openapi.Department>) {
            state.departments = departments
        },

        removeDepartment(state: any, id: string) {
            state.departments = state.departments.filter((d: openapi.Department) => d.id !== id)
        }
    },

    state: {
        departments: Array<openapi.Department>(),
    },

    getters: {
        departments(state: any) {
            return state.departments
        },
    },
};