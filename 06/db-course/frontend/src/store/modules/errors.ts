export default {
  actions: {
    async showError(ctx: any, error: string) {
      ctx.commit('saveError', error);
    },

    async hideError(ctx: any, error: string) {
      ctx.commit('deleteError', error);
    },
  },

  mutations: {
    saveError(state: any, msg: string) {
      state.errors.unshift(msg);
    },

    deleteError(state: any, msg: string) {
      state.errors = state.errors.filter((error: string) => (error !== msg));
    },
  },

  state: {
    errors: Array<string>(),
  },

  getters: {
    errors(state: any) {
      return state.errors;
    },
  },
};
