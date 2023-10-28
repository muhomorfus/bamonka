<script setup lang="ts">
import { RouterView } from 'vue-router'
import NavBar from './components/NavBar.vue'
import ErrorsBlock from "@/components/ErrorsBlock.vue";
</script>

<template>
  <NavBar v-if="!isLogin"/>

  <RouterView />

  <br>

  <ErrorsBlock/>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";

export default defineComponent({
  created() {
    this.getMe().then(_ => {
      if (!this.authorized && !this.isLogin) {
        window.location.replace('/login')
      }
    })
  },
  methods: {
    ...mapActions(['getMe']),
  },
  computed: {
    ...mapGetters(['authorized']),
    isLogin() {
      return this.$route.name === 'login'
    }
  }
});
</script>