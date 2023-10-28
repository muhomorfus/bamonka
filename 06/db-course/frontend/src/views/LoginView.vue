<template>
  <main class="container">
    <br>
    <div class="row">
      <div class="col-md-4"></div>
      <div class="col-md-4">
        <div class="card w-100">
          <div class="card-body">
            <h3 class="card-title mb-3">Login</h3>

            <form @submit.prevent="onSubmit">
              <div class="form-floating mb-3">
                <input type="text" class="form-control" id="nickname" placeholder="vasyan" v-model="nickname">
                <label for="nickname">Nickname</label>
              </div>
              <div class="form-floating mb-3">
                <input type="password" class="form-control" id="password" placeholder="password" v-model="password">
                <label for="password">Password</label>
              </div>

              <button class="btn btn-primary btn-lg w-100" type="submit">Login</button>
            </form>
          </div>
        </div>
      </div>
      <div class="col-md-4"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions} from "vuex";

export default defineComponent({
  data() {
    return {
      nickname: '',
      password: '',
    }
  },
  methods: {
    ...mapActions(['login']),
    onSubmit() {
      console.error(1)
      this.login({login: this.nickname, password: this.password})
    }
  }
});
</script>