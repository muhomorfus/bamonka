import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadEmployee(ctx: any) {
            try {
                const resp = await API.getEmployeeList()
                ctx.commit('saveEmployee', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async fireEmployee(ctx: any, id: string) {
            try {
                await API.fireEmployee(id)
                ctx.commit('moveFiredEmployee', id)

                ctx.dispatch('showError', 'Employer is fired.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async deleteEmployee(ctx: any, id: string) {
            try {
                await API.deleteEmployee(id)
                ctx.commit('removeEmployee', id)

                ctx.dispatch('showError', 'Employer is deleted.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        }
    },

    mutations: {
        saveEmployee(state: any, employee: Array<openapi.Employee>) {
            state.employee = employee.filter((e: openapi.Employee) => e.working)
            state.fired = employee.filter((e: openapi.Employee) => !e.working)
        },

        moveFiredEmployee(state: any, id: string) {
            const fired = state.employee.filter((e: openapi.Employee) => e.id === id)
            state.employee = state.employee.filter((e: openapi.Employee) => e.id !== id)
            state.fired = [...state.fired, ...fired]
        },

        removeEmployee(state: any, id: string) {
            state.employee = state.employee.filter((e: openapi.Employee) => e.id !== id)
            state.fired = state.fired.filter((e: openapi.Employee) => e.id !== id)
        }
    },

    state: {
        employee: Array<openapi.Employee>(),
        fired: Array<openapi.Employee>(),
    },

    getters: {
        employee(state: any) {
            return state.employee
        },

        fired(state: any) {
            return state.fired
        },
    },
};